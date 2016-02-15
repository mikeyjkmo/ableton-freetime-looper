#pragma once

#include <memory>
#include <cstdint>

#include "StartMessage.hpp"

namespace LiveFreetimeLooper
{
    class RunningLoop;

    class RecordingLoop final
    {
    private:
        // Length of loop relative to initial loop interval
        std::int32_t _interval;
        Command _command;
    public:
        RecordingLoop(Command command);
        void incrementInterval();
        RunningLoop moveToRunningLoop();
        std::int32_t getInterval();
    };

}
