#include "RecordingLoop.hpp"
#include "RunningLoop.hpp"

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

    std::int32_t RecordingLoop::getInterval()
    {
        return _interval;
    }
}
