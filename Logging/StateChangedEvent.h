#pragma once

#include <string>
#include <chrono>
#include "EventBase.h"

namespace AbletonProject
{

    class StateChangedEvent final : public EventBase
    {
    private:
        std::chrono::high_resolution_clock::time_point _time;
        std::string _detail;
        std::string _eventSource;
    public:
        StateChangedEvent(std::chrono::high_resolution_clock::time_point time, std::string detail, std::string eventSource);

        std::string getName() const override;
        std::chrono::high_resolution_clock::time_point getTime() const override;
        std::string getDetail() const override;
        LogLevel getLogLevel() const override;
        std::string getEventSource() const override;
    }; 
}
