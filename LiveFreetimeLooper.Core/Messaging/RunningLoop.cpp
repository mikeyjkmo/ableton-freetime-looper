#include "RunningLoop.hpp"
#include "StoppedLoop.hpp"
#include "Command.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    RunningLoop::RunningLoop(Command command, std::int32_t interval) :
        _interval(interval),
        _waitUntilNextRestart(0),
        _command(command)
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

    StoppedLoop RunningLoop::moveToStoppedLoop()
    {
        return StoppedLoop(_command, _interval);
    }

    Command RunningLoop::getCommand()
    {
        return _command;
    }

    std::int32_t RunningLoop::getInterval()
    {
        return _interval;
    }
}
