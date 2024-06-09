#pragma once

#include <functional>
#include <QVariant>
#include <QCommandLineOption>

namespace App {

template <typename T>
using HandlerFunc = std::function<void(const T&)>;

struct IOption
{
    IOption(const QCommandLineOption& option = QCommandLineOption(" "));

    virtual void handle(const QString&) const = 0;
    virtual ~IOption() = default;

    QCommandLineOption getOption() const;

private:
    QCommandLineOption _option;
};

template <typename T>
struct PropertyOption : public IOption
{
    PropertyOption(const PropertyOption&) = default;
    PropertyOption(const QCommandLineOption& userOption, const HandlerFunc<T>& handler = nullptr) 
        :  IOption(userOption), _handler(handler)
    {
    }

    void handle(const QString& value) const override
    {
        QVariant variant(value);
        if(variant.canConvert<T>()) {
            _handler(variant.value<T>());
        }
    }

private:
    const HandlerFunc<T> _handler;
};

}