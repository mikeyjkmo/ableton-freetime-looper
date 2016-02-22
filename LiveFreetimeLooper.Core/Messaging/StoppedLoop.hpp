#pragma once

#include <memory>
#include <cstdint>

#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class RunningLoop;

    class StoppedLoop final
    {
    private:    
        // Length of loop relative to initial loop interval
        const std::int32_t _interval; 
        Command _command;
    public:
        StoppedLoop(Command command, std::int32_t interval);
        RunningLoop moveToRunningLoop();
        Command getCommand();
        std::int32_t getInterval();
    };

}