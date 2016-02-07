#include "StateChangedEvent.hpp"

namespace LiveFreetimeLooper
{
    StateChangedEvent::StateChangedEvent(std::string detail, std::string eventSource) :
        _time(std::chrono::high_resolution_clock::now()),
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
