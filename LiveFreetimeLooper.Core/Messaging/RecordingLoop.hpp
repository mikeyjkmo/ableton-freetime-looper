#pragma once

#include <memory>
#include <cstdint>

#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class RunningLoop;
    class StoppedLoop;

    class RecordingLoop final
    {
    private:
        // Length of loop relative to initial loop interval
        std::int32_t _interval;
        Command _command;
    public:
        explicit RecordingLoop(Command command);
        void incrementInterval();
        RunningLoop moveToRunningLoop();
        StoppedLoop moveToStoppedLoop();
        std::int32_t getInterval();
    };

}
