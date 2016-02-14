#include "MessageDispatchedEvent.hpp"
#include "../Messaging/StartMessage.hpp"

namespace LiveFreetimeLooper
{
    MessageDispatchedEvent::MessageDispatchedEvent(StartMessage& message, std::string eventSource) :
        _time(std::chrono::high_resolution_clock::now()),
        _detail(MessageDispatchedEvent::createDetail(message)),
        _eventSource(eventSource)
    {
    }

    std::string MessageDispatchedEvent::createDetail(StartMessage& message)
    {
        auto detail = std::string("Payload:");
        for (auto& c : message.payload)
        {
            detail += std::string(" ");
            detail += std::to_string((unsigned int)c);
        }

        return detail;
    }

    std::string MessageDispatchedEvent::getName() const
    {
        return std::string("Message Dispatched");
    }

    std::chrono::high_resolution_clock::time_point MessageDispatchedEvent::getTime() const
    {
        return _time;
    }

    std::string MessageDispatchedEvent::getDetail() const
    {
        return _detail;
    }

    LogLevel MessageDispatchedEvent::getLogLevel() const
    {
        return LogLevel::DEBUG;
    }

    std::string MessageDispatchedEvent::getEventSource() const
    {
        return _eventSource;
    }
}
