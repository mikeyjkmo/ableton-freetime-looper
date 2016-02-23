#include "RecordingLoop.hpp"
#include "RunningLoop.hpp"
#include "StoppedLoop.hpp"

namespace LiveFreetimeLooper
{
    RecordingLoop::RecordingLoop(Command command) :
        _interval(0),
        _command(command)
    {

    }

    void RecordingLoop::incrementInterval()
    {
        ++_interval;
    }

    RunningLoop RecordingLoop::moveToRunningLoop()
    {
        return RunningLoop(_command, _interval);
    }

    StoppedLoop RecordingLoop::moveToStoppedLoop()
    {
        return StoppedLoop(_command, _interval);
    }

    std::int32_t RecordingLoop::getInterval()
    {
        return _interval;
    }
}