#pragma once

#include <memory>
#include <cstdint>

#include "StartMessage.hpp"

namespace LiveFreetimeLooper
{
    class Command;

    class RunningLoop final
    {
    private:    
        // Length of loop relative to initial loop interval
        const std::int32_t _interval; 
        // Counts down when checkIfRestartRequired and decrementNextRestartWait called
        // st checkIfRestartRequired returns true every _interval times
        std::int32_t _waitUntilNextRestart;
        Command _command;
    public:
        RunningLoop(Command command, std::int32_t interval);
        bool checkIfRestartRequired();
        void decrementNextRestartWait();
        Command getCommand();
        std::int32_t getInterval();
    };

}
