#include "ProcessInfo.hpp"
#include <cstdint>
#include "OsDetection.hpp"

#if defined(OS_UNIX)

#include <QDir>
#include <QHash>
#include <QProcess>
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>

namespace Process {

namespace {

constexpr std::string_view processesPath("/proc");
constexpr std::string_view cmdlineFileName("cmdline");
constexpr std::string_view statusFileName("status");
constexpr std::string_view usageMemoryVar("VmRSS");
constexpr std::string_view nativeNameVar("Name");

class StatusFileParser
{
    QFile _statusFile;
    QHash<QString, QString> _values;

public:
    StatusFileParser() = default;

    bool openFile(const QString& path)
    {
        _statusFile.setFileName(path);
        return _statusFile.open(QIODevice::ReadOnly);
    }

    bool parse()
    {
        auto content = _statusFile.readAll();
        int startIndex = 0;
        int propertyIndex = 0;

        int lineIndex = content.indexOf("\n", startIndex);
        while(lineIndex != -1) {
            propertyIndex = content.indexOf(":", startIndex);
            auto property = content.sliced(startIndex, propertyIndex - startIndex);
            auto value = content.sliced(propertyIndex + 1, lineIndex - propertyIndex).trimmed();
            _values[property] = value;
            startIndex = lineIndex + 1;
            lineIndex = content.indexOf("\n", startIndex);
            // qDebug() << property << value;
        }
        return isParsed();
    }

    bool isParsed() const
    {
        return !_values.empty();
    }

    QString getValue(const QString& propertyName) const
    {
        return _values.value(propertyName);
    }
};

}

ProcessInfoList ProcessInfo::collect(const QRegularExpression& regex)
{
    ProcessInfoList processList;
    QDir processesDir(processesPath.data());
    auto files = processesDir.entryList();

    for(const auto& file : files) {
        bool isPid;
        std::uint32_t pid = file.toInt(&isPid);
        if(!isPid) {
            continue;
        }
        QDir processDir(processesDir.absoluteFilePath(file));
        QFile commandFile(processDir.absoluteFilePath(cmdlineFileName.data()));
        if(!commandFile.open(QIODevice::ReadOnly)) {
            continue;
        }
        auto content = commandFile.readAll();
        int endIndex = content.indexOf('\0');
        if(endIndex > 0) {
            auto firstArg = QString::fromLocal8Bit(content.data(), endIndex);
            content.replace('\0', ' ');
        }
        QString commandLine = QString::fromUtf8(content).trimmed();
        if(commandLine.isEmpty()) {
            continue;
        }
        if(!regex.match(commandLine).hasMatch()) {
            continue;
        }
        processList.push_back(ProcessInfo{pid, commandLine});
    }

    return processList;
}

std::optional<uint32_t> ProcessInfo::usageMemoryKB() const
{
    QDir statusDir(
        QString("%1/%2")
        .arg(processesPath.data())
        .arg(QString::number(_pid))
    );
    StatusFileParser parser;
    if(!parser.openFile(statusDir.absoluteFilePath(statusFileName.data()))){
        qWarning() << "Error opening the status file";
        return std::nullopt;
    }
    if(!parser.parse()) {
        qWarning() << "Error when parse status file";
        return std::nullopt;
    }
    auto memoryValue = parser.getValue(usageMemoryVar.data());
    bool isDigit;
    uint32_t result = memoryValue.chopped(3).toUInt(&isDigit);
    if(!isDigit) {
        qWarning() << "Error when getting value";
        return std::nullopt;
    }
    return std::optional<uint32_t>(result);
}

std::optional<QString> ProcessInfo::nativeName() const
{
    QDir statusDir(
        QString("%1/%2")
        .arg(processesPath.data())
        .arg(QString::number(_pid))
    );
    StatusFileParser parser;
    if(!parser.openFile(statusDir.absoluteFilePath(statusFileName.data()))){
        qWarning() << "Error opening the status file";
        return std::nullopt;
    }
    if(!parser.parse()) {
        qWarning() << "Error when parse status file";
        return std::nullopt;
    }
    return std::optional<QString>(parser.getValue(nativeNameVar.data()));
}

}

#endif