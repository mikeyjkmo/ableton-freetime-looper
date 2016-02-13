#include "RunningLoop.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    RunningLoop::RunningLoop(std::unique_ptr<Message> controlMessage, std::int32_t interval) :
        _interval(interval),
        _waitUntilNextRestart(0),
        _controlMessage(std::move(controlMessage))
    {
        if (_interval < 1) throw std::runtime_error("Interval must be a positive number");
    }

    bool RunningLoop::checkIfRestartRequired()
    {
        return _waitUntilNextRestart < 1;
    }

    void RunningLoop::decrementNextRestartWait()
    {
        if (_waitUntilNextRestart < 1)
        {
            _waitUntilNextRestart = _interval - 1;
            return;
        }

        --_waitUntilNextRestart;
    }

    Message* RunningLoop::getControlMessage()
    {
        return _controlMessage.get();
    }

    std::int32_t RunningLoop::GetInterval()
    {
        return _interval;
    }
}
