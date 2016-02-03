#pragma once
#include <memory>
#include <string>

#include "Messaging/IMessageDispatcher.h"
#include "Messaging/ILoopTracker.h"
#include "Logging/IEventLogger.h"
#include "Utilities/IAsyncTimerFactory.h"

namespace LiveFreetimeLooper
{
    class StateResources final
    {
    public:
        IMessageDispatcher& messageDispatcher;
        ILoopTracker& loopTracker;
        IEventLogger& logger;
        IAsyncTimerFactory& asyncTimerFactory;

        StateResources(
            IMessageDispatcher& messageDispatcher, ILoopTracker& loopTracker, IEventLogger& logger, IAsyncTimerFactory& asyncTimerFactory)
            : messageDispatcher(messageDispatcher),
              loopTracker(loopTracker),
              logger(logger),
              asyncTimerFactory(asyncTimerFactory)
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
