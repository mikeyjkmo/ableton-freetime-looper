#pragma once

#include <string>
#include <chrono>
#include "EventBase.h"
#include "Messaging/Message.h"

namespace LiveFreetimeLooper
{

    class MessageDispatchedEvent final : public EventBase
    {
    private:
        std::chrono::high_resolution_clock::time_point _time;
        std::string _detail;
        std::string _eventSource;
        static std::string createDetail(Message& message);
    public:
        MessageDispatchedEvent(Message& message, std::string eventSource);

        std::string getName() const override;
        std::chrono::high_resolution_clock::time_point getTime() const override;
        std::string getDetail() const override;
        LogLevel getLogLevel() const override;
        std::string getEventSource() const override;
    }; 
}
