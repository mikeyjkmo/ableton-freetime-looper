#pragma once

#include <memory>
#include <cstdint>

#include "Message.hpp"

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
        std::unique_ptr<Message> _controlMessage;
    public:
        RunningLoop(std::unique_ptr<Message> controlMessage, std::int32_t interval);
        bool checkIfRestartRequired();
        void decrementNextRestartWait();
        Message* getControlMessage();
        std::int32_t getInterval();
    };

}
