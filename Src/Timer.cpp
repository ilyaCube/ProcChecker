#include "Timer.h"

namespace Time {

TimerBase::TimerBase(const std::function<void()>& func) :
    _func(func)
{
    std::lock_guard<std::recursive_mutex> globalLock(cb().globalMutex);

    for (;;) {
        _id = cb().nextId++;
        auto it = cb().timers.find(_id);
        if (it == cb().timers.end()) {
            cb().timers.insert(it, std::pair<std::uint32_t, TimerBase *>{_id, this});
            return;
        }
    }
}

TimerBase::~TimerBase()
{
    try {
        std::lock_guard<std::recursive_mutex> globalLock(cb().globalMutex);
        std::lock_guard<std::mutex> lock(_funcMutex);
        cb().timers.erase(_id);
    }
    catch (const std::exception& ex) {
        (void)ex;
        #if DEBUG
            printf("%s", ex.what());
        #endif
    }
}

void TimerBase::stop()
{
    const std::lock_guard<std::mutex> lock(_mutex);

    if (!_isRunning.exchange(false)) {
        return;
    }

    stopImpl();
}

bool TimerBase::isRunning() const
{
    return _isRunning;
}

TimerBase::ControlBlock& TimerBase::cb()
{
    static ControlBlock cb;
    return cb;
}

void TimerBase::onTimer(std::uint32_t id)
{
    cb().globalMutex.lock();

    auto it = cb().timers.find(id);
    if (it == cb().timers.end()) {
        cb().globalMutex.unlock();
        return;
    }

    auto that = it->second;

    auto func = that->_func;
    if (!func) {
        return;
    }

    bool isActive = that->_isRunning;

    std::unique_lock<std::mutex> funcLock(that->_funcMutex, std::try_to_lock);

    cb().globalMutex.unlock();

    if (isActive && funcLock.owns_lock()) {
        func();
    }
}

}
