#include "ProcessInfo.hpp"

namespace Process {

ProcessInfo::ProcessInfo(uint32_t pid, const QString& commandLine)
    : _pid(pid), _commandLine(commandLine)
{
}

std::optional<QString> ProcessInfo::commandLine(const QRegularExpression& regex) const
{
    return _commandLine;
}

}