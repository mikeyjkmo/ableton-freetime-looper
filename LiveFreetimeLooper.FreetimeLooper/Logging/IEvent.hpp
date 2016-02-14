#pragma once

#include <string>
#include <chrono>

namespace LiveFreetimeLooper
{
    enum LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

    class IEvent
    {
    protected:
        IEvent() {};
    public:
        virtual ~IEvent() = default;
        virtual std::string getName() const = 0;
        virtual std::chrono::high_resolution_clock::time_point getTime() const = 0;
        virtual std::string getDetail() const = 0;
        virtual LogLevel getLogLevel() const = 0;
        virtual std::string getEventSource() const = 0;
    };
}
