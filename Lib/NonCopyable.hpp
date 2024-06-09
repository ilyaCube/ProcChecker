#pragma once

namespace Common {

struct NonCopyable
{
    virtual ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;

    const NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
};

}
