#pragma once

#include <optional>
#include <QList>
#include <QString>
#include <QRegularExpression>

namespace Process {

class ProcessInfo;
typedef QList<ProcessInfo> ProcessInfoList;

class ProcessInfo
{
public:
    ProcessInfo(uint32_t pid, const QString& commandLine);

    [[nodiscard]] static ProcessInfoList collect(const QRegularExpression& regex = {});

    [[nodiscard]] std::optional<uint32_t> usageMemoryKB() const;

    [[nodiscard]] std::optional<QString> nativeName() const;

    [[nodiscard]] std::optional<QString> commandLine(const QRegularExpression& regex = {}) const;

private:
    uint32_t _pid;
    QString _commandLine;
};

}