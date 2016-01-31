#pragma once
#include <memory>
#include <string>

#include "Messaging/IMessageDispatcher.h"
#include "Messaging/LoopTracker.h"
#include "Logging/IEventLogger.h"

namespace LiveFreetimeLooper
{
    class StateResources final
    {
    public:
        IMessageDispatcher& messageDispatcher;
        LoopTracker& loopTracker;
        IEventLogger& logger;

        StateResources(
            IMessageDispatcher& messageDispatcher, LoopTracker& loopTracker, IEventLogger& logger)
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
