#include "CmdLineOption.hpp"

namespace App {

IOption::IOption(const QCommandLineOption& option) : 
    _option(option)
{
}

QCommandLineOption IOption::getOption() const
{
    return _option;
}

}