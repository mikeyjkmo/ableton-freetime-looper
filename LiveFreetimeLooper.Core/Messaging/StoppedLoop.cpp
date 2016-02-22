#include "StoppedLoop.hpp"
#include "RunningLoop.hpp"
#include "Command.hpp"
#include <stdexcept>

namespace LiveFreetimeLooper
{
    StoppedLoop::StoppedLoop(Command command, std::int32_t interval) :
        _interval(interval),
        _command(command)
    {
        if (_interval < 1) throw std::runtime_error("Interval must be a positive number");
    }

    RunningLoop StoppedLoop::moveToRunningLoop()
    {
        return RunningLoop(_command, _interval);
    }

    Command StoppedLoop::getCommand()
    {
        return _command;
    }

    std::int32_t StoppedLoop::getInterval()
    {
        return _interval;
    }
}