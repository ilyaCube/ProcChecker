#include "AppLauncher.hpp"

#include <iostream>
#include "CmdLineParser.hpp"

namespace App {

AppLauncher::AppLauncher(int argc, char** argv)
    : QApplication(argc, argv),
    _timer(
        [this] {
            auto must = findMustHeavist(QRegularExpression{_args.processRegex});
            printProcess(must);
        }
    )
{
    App::CommandLineParser parser("ProcChecker");
    auto printAndExit = [] (const QString& text) {
        std::clog << text.toStdString() << std::endl;
        std::exit(0);
    };

    parser.addCustomOption<QString>(
        {{"r", "regex"}, "Match process", "RegEx"},
        [this](const QString& value){
            _args.processRegex = value;
        }
    )
    .addCustomOption<uint32_t>(
        {{"u", "update"}, "Update frequency", "ms"},
        [this](uint32_t value){
            _args.updateMs = 0.01 * value;
        }
    )
    .addVersionOption(printAndExit)
    .addHelpOption(printAndExit);

    parser.process(argc, argv, printAndExit);
}

AppLauncher::~AppLauncher()
{
    _timer.stop();
}

int AppLauncher::start()
{
    _timer.start(Time::Millis{_args.updateMs});
    return QApplication::exec();
}

std::optional<Process::ProcessInfo> AppLauncher::findMustHeavist(const QRegularExpression& regex) const
{
    auto collected = Process::ProcessInfo::collect(regex);
    std::optional<std::pair<uint32_t, Process::ProcessInfo>> mustHeaviest = std::nullopt;
    for(const auto& process : collected) {
        auto memoryOpt = process.usageMemoryKB();
        auto nameOpt = process.commandLine();
        if(!memoryOpt.has_value() || !nameOpt.has_value()) {
            continue;
        }
        auto memory = memoryOpt.value();
        auto name = nameOpt.value();
        if(mustHeaviest.has_value()) {
            auto [prevMemory, oldInfo] = mustHeaviest.value();
            if(prevMemory > memory) {
                continue;
            }
        }
        mustHeaviest = std::pair<uint32_t, Process::ProcessInfo>(memory, process);
    }
    return mustHeaviest.has_value() ? std::optional<Process::ProcessInfo>(mustHeaviest->second) : std::nullopt;
}

void AppLauncher::printProcess(const std::optional<Process::ProcessInfo>& processOpt) const
{
    system("clear");
    if(processOpt.has_value()) {
        auto process = processOpt.value();
        auto memoryOpt = process.usageMemoryKB();
        auto nameOpt = process.nativeName();
        if(memoryOpt.has_value()) {
            qInfo() << "Memory:" << memoryOpt.value() << "kB";
        }
        if(nameOpt.has_value()) {
            qInfo() << "Name:" << nameOpt.value();
        }
    } else {
        qInfo() << "UNKNOWN";
    }
}

}