#include "StateChangedEvent.h"

namespace AbletonProject
{
    StateChangedEvent::StateChangedEvent(std::chrono::high_resolution_clock::time_point time, std::string detail, std::string eventSource) :
        _time(time),
        _detail(detail),
        _eventSource(eventSource)
    {
    }

    std::string StateChangedEvent::getName() const
    {
        return std::string("State Changed");
    }

    std::chrono::high_resolution_clock::time_point StateChangedEvent::getTime() const
    {
        return _time;
    }

    std::string StateChangedEvent::getDetail() const
    {
        return _detail;
    }

    LogLevel StateChangedEvent::getLogLevel() const
    {
        return LogLevel::INFO;
    }

    std::string StateChangedEvent::getEventSource() const
    {
        return _eventSource;
    }
}
