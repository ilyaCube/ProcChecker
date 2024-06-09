#pragma once

#include <memory>
#include <vector>
#include <QVariant>
#include <QCommandLineParser>
#include "CmdLineOption.hpp"

namespace App {

struct CommandLineParser
{
private:
    QCommandLineParser _parser;
    const QStringList _ignoreOptions;
    std::vector<std::unique_ptr<IOption>> _options;
    const QString _appDescription;
    const QString _appVersion;

    void insertOption(std::unique_ptr<IOption>&& newOption);

public:
    CommandLineParser(const QString& appDescription = "", const QString& appVersion = "", const QStringList& ignoreOptions = {"style", "stylesheet", "widgetcount", "reverse", "qmljsdebugger"});

    void process(int argc, char **argv, const HandlerFunc<QString>& errorHandler = nullptr);
    void process(const QStringList& args, const HandlerFunc<QString>& errorHandler = nullptr);

    template <typename T>
    CommandLineParser& addCustomOption(const QCommandLineOption& userOption, const HandlerFunc<T>& handler)
    {
        auto newOption = std::make_unique<PropertyOption<T>>(userOption, handler);
        insertOption(std::move(newOption));
        return *this;
    }

    CommandLineParser& addHelpOption(const HandlerFunc<QString>& handler);
    CommandLineParser& addVersionOption(const HandlerFunc<QString>& handler);

    QString collectHelpText() const;
};

}