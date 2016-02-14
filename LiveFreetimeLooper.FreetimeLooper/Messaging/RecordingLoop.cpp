#include "RecordingLoop.hpp"
#include "RunningLoop.hpp"

namespace LiveFreetimeLooper
{
    RecordingLoop::RecordingLoop(std::unique_ptr<Message> controlMessage) :
        _interval(0),
        _controlMessage(std::move(controlMessage))
    {

    }

    void RecordingLoop::incrementInterval()
    {
        ++_interval;
    }

    RunningLoop RecordingLoop::MoveToRunningLoop()
    {
        return RunningLoop(std::move(_controlMessage), _interval);
    }

    std::int32_t RecordingLoop::getInterval()
    {
        return _interval;
    }
}
