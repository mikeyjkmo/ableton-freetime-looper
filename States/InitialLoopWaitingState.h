#pragma once
#include <memory>
#include <string>

#include "Utilities/Stopwatch.h"
#include "Messaging/MessageDispatcher.h"
#include "StateBase.h"
#include "Messaging/Message.h"
#include "Logging/EventLogger.h"
#include "Logging/StateChangedEvent.h"

// Next states:
#include "InitialLoopState.h"
#include "CreatedState.h"

namespace AbletonProject
{
    // Waiting for first message to start Initial Loop
    class InitialLoopWaitingState final : public StateBase
    {
    private:
        MessageDispatcher& _messageDispatcher;
        EventLogger& _logger;
    public:
        InitialLoopWaitingState(MessageDispatcher& messageDispatcher, EventLogger& logger);

        void handle(std::unique_ptr<StateBase>& state, Message message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
