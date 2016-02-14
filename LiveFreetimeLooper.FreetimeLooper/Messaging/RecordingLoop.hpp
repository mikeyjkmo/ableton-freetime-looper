#pragma once

#include <memory>
#include <cstdint>

#include "StartMessage.hpp"

namespace LiveFreetimeLooper
{
    class RunningLoop;

    class RecordingLoop
    {
    private:
        // Length of loop relative to initial loop interval
        std::int32_t _interval;
        std::unique_ptr<StartMessage> _controlMessage;
    public:
        RecordingLoop(std::unique_ptr<StartMessage> controlMessage);
        void incrementInterval();
        RunningLoop moveToRunningLoop();
        std::int32_t getInterval();
    };

}
