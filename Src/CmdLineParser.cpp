#include "CmdLineParser.hpp"
#include "CmdLineOption.hpp"

namespace App {

CommandLineParser::CommandLineParser(const QString& appDescription, const QString& appVersion, const QStringList& ignoreOptions)
    : _appDescription(appDescription), _appVersion(appVersion), _ignoreOptions(ignoreOptions)
{
    _parser.setApplicationDescription(appDescription);
    _parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
}

void CommandLineParser::insertOption(std::unique_ptr<IOption>&& newOption)
{
    _parser.addOption(newOption->getOption());
    _options.emplace_back(std::move(newOption));
}

QString CommandLineParser::collectHelpText() const
{
    auto helpText = QString("%1\nOptions\n").arg(_appDescription);
    for(const auto& optionPtr : _options) {
        const auto qOption = optionPtr->getOption();
        QString formattedOption;
        for(auto key : qOption.names()) {
            if(key.size() == 1) {
                formattedOption.append(key.prepend(" -"));
            } else if(key.size() > 1) {
                formattedOption.append(key.prepend(" --"));
            }
        }
        auto valueName = qOption.valueName();
        if(!valueName.isEmpty()) {
            formattedOption.append("=").append(valueName);
        }
        formattedOption.append("\r\t\t\t");
        helpText.append(QString("%1\t%2.\n").arg(formattedOption).arg(qOption.description()));
    }
    return helpText;
}

void CommandLineParser::process(const QStringList& args, const HandlerFunc<QString>& errorHandler)
{
    if(!_parser.parse(args)) {
        const auto unknownArgs = _parser.unknownOptionNames();
        for(const auto& unknownArg : unknownArgs) {
            if(!_ignoreOptions.contains(unknownArg)) {
                errorHandler(_parser.errorText());
            }
        }
    }

    for(auto& optionPtr : _options) {
        const auto qOption = optionPtr->getOption();
        if(_parser.isSet(qOption)) {
            auto value = _parser.value(qOption);
            optionPtr->handle(value);
        }
    }
}

void CommandLineParser::process(int argc, char **argv, const HandlerFunc<QString>& errorHandler)
{
    QStringList argList;
    for (int i = 0; i < argc; ++i) {
        argList << argv[i];
    }
    return process(argList, errorHandler);
}

CommandLineParser& CommandLineParser::addHelpOption(const HandlerFunc<QString>& handler)
{
    auto helpOption = std::make_unique<PropertyOption<QString>>(
        QCommandLineOption{{"h", "help"}, "Displays help on commandline options"},
        [this, handler](const QString& value) {
            handler(collectHelpText());
        }
    );
    insertOption(std::move(helpOption));
    return *this;
}

CommandLineParser& CommandLineParser::addVersionOption(const HandlerFunc<QString>& handler)
{
    auto versionOption = std::make_unique<PropertyOption<QString>>(
        QCommandLineOption{{"v", "version"}, "Displays version information"},
        [this, handler](const QString& value) {
            handler(_appVersion);
        }
    );
    insertOption(std::move(versionOption));
    return *this;
}

}