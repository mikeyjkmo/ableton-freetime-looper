#pragma once

#include <memory>
#include <cstdint>

#include "Message.hpp"

namespace LiveFreetimeLooper
{
    class RunningLoop;

    class RecordingLoop
    {
    private:    
        // Length of loop relative to initial loop interval
        std::int32_t _interval; 
        std::unique_ptr<Message> _controlMessage;
    public:
        RecordingLoop(std::unique_ptr<Message> controlMessage);
        void incrementInterval();
        RunningLoop MoveToRunningLoop();
        std::int32_t RecordingLoop::GetInterval();
    };

}
