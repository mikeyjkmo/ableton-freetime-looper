#pragma once

#include <memory>
#include <cstdint>

#include "StartMessage.hpp"

namespace LiveFreetimeLooper
{

    class RunningLoop
    {
    private:    
        // Length of loop relative to initial loop interval
        const std::int32_t _interval; 
        // Counts down when checkIfRestartRequired and decrementNextRestartWait called
        // st checkIfRestartRequired returns true every _interval times
        std::int32_t _waitUntilNextRestart;
        std::unique_ptr<StartMessage> _controlMessage;
    public:
        RunningLoop(std::unique_ptr<StartMessage> controlMessage, std::int32_t interval);
        bool checkIfRestartRequired();
        void decrementNextRestartWait();
        StartMessage* getControlMessage();
        std::int32_t getInterval();
    };

}
