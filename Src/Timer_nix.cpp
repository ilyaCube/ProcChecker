#include "OsDetection.hpp"

#if defined(OS_LINUX)

#include "Timer.h"
#include "Exceptions.hpp"

namespace Time {

timespec toTimespec(double duration, bool isInitial)
{
    // +1ns for initial is used because Linux consider 0 as no timer
    auto s = (long) duration;
    auto ns = (long) ((duration - s) * 1e9);
    return timespec{s, ns + (isInitial ? 1 : 0)};
}

Timer::Timer(const std::function<void()>& func) :
    TimerBase(func)
{}

Timer::~Timer()
{
    stop();
}

void Timer::startImpl(double initial, double periodSec)
{
    ::sigevent se;
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_int = _id;
    se.sigev_notify_function = &callback;
    se.sigev_notify_attributes = nullptr;

    if (::timer_create(CLOCK_MONOTONIC, &se, &_timer) != 0)
        THROW(std::runtime_error, "::timer_create error");

    itimerspec ts{toTimespec(periodSec, false), toTimespec(initial, true)};

    if (::timer_settime(_timer, 0, &ts, nullptr) != 0)
        THROW(std::runtime_error, "::timer_settime error");
}

void Timer::stopImpl()
{
    ::itimerspec ts{0, 0, 0, 0};
    if (::timer_settime(_timer, 0, &ts, nullptr) != 0)
        THROW(std::runtime_error, "::timer_settime error");

    if (::timer_delete(_timer) != 0)
        THROW(std::runtime_error, "::timer_delete error");

    _timer = ::timer_t();
}

void Timer::callback(sigval_t sigval)
{
    onTimer(sigval.sival_int);
}

}

#endif
