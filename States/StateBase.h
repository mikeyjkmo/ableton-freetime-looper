#pragma once
#include <memory>
#include <string>

#include "Messaging/MessageDispatcher.h"
#include "Messaging/LoopTracker.h"
#include "Logging/EventLogger.h"

namespace AbletonProject
{
    class StateResources
    {
    public:
        MessageDispatcher& messageDispatcher;
        LoopTracker& loopTracker;
        EventLogger& logger;

        StateResources(
            MessageDispatcher& messageDispatcher, LoopTracker& loopTracker, EventLogger& logger)
            : messageDispatcher(messageDispatcher),
              loopTracker(loopTracker),
              logger(logger)
        {
        }
    };

    class StateBase
    {
    protected:
        StateBase() {};
    public:
        virtual ~StateBase() = default;
        virtual void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) = 0;
        virtual void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) = 0;
    };

}
