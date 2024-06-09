#pragma once

#include <mutex>
#include <atomic>
#include <functional>
#include "NonCopyable.hpp"
#include "TimeHelpers.hpp"

namespace Time {

class TimerBase : Common::NonCopyable
{
    struct ControlBlock
    {
        std::uint32_t nextId{0};
        std::recursive_mutex globalMutex;
        std::unordered_map<std::uint32_t, TimerBase *> timers;
    };


public:
    explicit TimerBase(const std::function<void()>& func);

    // Block till timer will be cancelled and last callback finished
    ~TimerBase() override;

    void stop();
    bool isRunning() const;

    template<typename Duration>
    inline void start(Duration period)
    {
        start(period, period);
    }
    
    template<typename Duration, typename Duration2 = Duration>
    void start(Duration initial, Duration2 period)
    {
        const std::lock_guard<std::mutex> lock(_mutex);

        if (_isRunning.exchange(true)) {
            return;
        }

        startImpl(
            Time::SecondsF(initial).count(),
            Time::SecondsF(period).count()
        );
    }

    template<typename Duration>
    inline void restart(Duration period)
    {
        restart(period, period);
    }

    template<typename Duration, typename Duration2 = Duration>
    void restart(Duration initial, Duration2 period)
    {
        const std::lock_guard<std::mutex> lock(_mutex);

        if (_isRunning.exchange(true)) {
            stopImpl();
        }

        startImpl(
            Time::SecondsF(initial).count(),
            Time::SecondsF(period).count()
        );
    }

protected:
    const std::function<void()> _func;
    std::mutex _funcMutex;
    std::mutex _mutex;
    std::uint32_t _id;
    std::atomic_bool _isRunning{false};

    inline static ControlBlock& cb();

    static void onTimer(std::uint32_t id);

    virtual void startImpl(double initial, double periodSec) = 0;

    virtual void stopImpl() = 0;
};

}


// Include one of platform-dependent implementations

#if defined(_WIN32)

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <Windows.h>
#undef min
#undef max

namespace Unicore { namespace Library { namespace Time {

class Timer : public TimerBase
    {
    public:
        explicit Timer(const std::function<void()>& func = nullptr);

        ~Timer() override;

    protected:
        void startImpl(double initial, double periodSec) override;

        void stopImpl() override;

    private:
        HANDLE _handle{nullptr};
        static void callback(void *id);
    };

}}}

#else

#include <signal.h>
#include <time.h>

namespace Time {

class Timer : public TimerBase
{
public:
    explicit Timer(const std::function<void()>& func = nullptr);

    ~Timer() override;

protected:
    void startImpl(double initial, double periodSec) override;

    void stopImpl() override;

private:
    ::timer_t _timer;

    static void callback(sigval_t sigval);
};

}

#endif
