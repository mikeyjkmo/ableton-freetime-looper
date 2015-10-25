#pragma once
#include <memory>
#include <string>

#include "Utilities/Stopwatch.h"
#include "Messaging/MessageDispatcher.h"
#include "StateBase.h"
#include "Messaging/Message.h"
#include "Logging/EventLogger.h"
#include "Logging/StateChangedEvent.h"

// Next state:
#include "InitialLoopWaitingState.h"

namespace AbletonProject
{
    // Waiting for stdIn to signal that intial loop start message is coming
    class CreatedState final : public StateBase
    {
    private:
        MessageDispatcher& _messageDispatcher;
        EventLogger& _logger;
    public:
        CreatedState(MessageDispatcher& messageDispatcher, EventLogger& logger);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
