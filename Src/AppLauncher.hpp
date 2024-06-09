#pragma once

#include <QApplication>
#include "Timer.h"
#include "TimeHelpers.hpp"
#include "ProcessInfo.hpp"

namespace App {

class AppLauncher : private QApplication
{
public:
    static constexpr Time::Millis DEFAULT_FREQ = Time::Millis{500};
    static constexpr std::string_view DEFAULT_REGEX = "bin";

    struct CmdArgs
    {
        QString processRegex = DEFAULT_REGEX.data();
        uint32_t updateMs = DEFAULT_FREQ.count();
    };

    AppLauncher(int argc, char** argv);
    ~AppLauncher();

    int start();

    [[nodiscard]] std::optional<Process::ProcessInfo> findMustHeavist(const QRegularExpression& regex) const;
    void printProcess(const std::optional<Process::ProcessInfo>& processOpt)  const;

private:
    Time::Timer _timer;
    CmdArgs _args;
};

}