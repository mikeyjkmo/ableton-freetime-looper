#pragma once

#include <memory>
#include <cstdint>

#include "Message.h"

namespace AbletonProject
{

    class Loop
    {
    private:    
        // Length of loop relative to initial loop interval
        std::int32_t _interval; 
        // Counts down when checkIfRestartRequired and decrementNextRestartWait called
        // st checkIfRestartRequired returns true every _interval times
        std::int32_t _waitUntilNextRestart;
        std::unique_ptr<Message> _controlMessage;
    public:
        Loop(std::unique_ptr<Message> controlMessage);
        void incrementInterval();
        bool checkIfRestartRequired();
        void decrementNextRestartWait();
        Message* getControlMessage();
    };

}