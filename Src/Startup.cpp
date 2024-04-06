#include <thread>
#include <QApplication>
#include <QProcess>
#include "ProcessInfo.hpp"
#include "TimeHelpers.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    while(1) {
        auto collected = Process::ProcessInfo::collect(QRegularExpression{"Apps/Viewer/CMakeFiles"});
        std::optional<std::pair<uint32_t, QString>> mustHeaviest = std::nullopt;
        for(const auto& process : collected) {
            auto memoryOpt = process.usageMemoryKB();
            auto nameOpt = process.commandLine();
            if(!memoryOpt.has_value() || !nameOpt.has_value()) {
                continue;
            }
            auto memory = memoryOpt.value();
            auto name = nameOpt.value();
            if(mustHeaviest.has_value()) {
                auto [oldMemory, oldName] = mustHeaviest.value();
                if(oldMemory > memory) {
                    continue;
                }
            }
            mustHeaviest = std::pair<uint32_t, QString>(memory, name);
        }
        if(mustHeaviest.has_value()) {
            auto [memory, name] = mustHeaviest.value();
            qDebug() << "Must heaviest:" << memory << name;
        } else {
            qDebug() << "Not value";
        }
        std::this_thread::sleep_for(Time::Seconds(1));
        system("clear");
    }
    app.quit();

    return 0;
}