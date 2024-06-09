#include "OsDetection.hpp"

#if defined(OS_WINDOWS)

#include "Timer.h"

namespace Time {

Timer::Timer(const std::function<void()>& func) :
    TimerBase(func)
{}

Timer::~Timer()
{
    stop();
}

void Timer::startImpl(double initial, double periodSec)
{
    auto ok = ::CreateTimerQueueTimer(
        &_handle,
        nullptr,
        (WAITORTIMERCALLBACK) callback,
        (void *) (long long) _id,
        (DWORD) (initial * 1e3 + 0.5),
        (DWORD) (periodSec * 1e3 + 0.5),
        WT_EXECUTEDEFAULT // May be use WT_EXECUTELONGFUNCTION which creates separate thread
    );

    if (ok == 0)
        THROW(TimeException, "Can't create timer");
}

void Timer::stopImpl()
{
    ::DeleteTimerQueueTimer(nullptr, _handle, NULL); // NULL - does not wait, INVALID_HANDLE_VALUE - wait untill timer is cancelled and callback is really finished
    _handle = nullptr;
}

void Timer::callback(void *id)
{
    onTimer((std::uint32_t) (unsigned long long) id);
}

}

#endif
