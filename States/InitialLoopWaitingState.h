#pragma once
#include <memory>
#include <string>

#include "Utilities/Stopwatch.h"
#include "Messaging/MessageDispatcher.h"
#include "StateBase.h"
#include "Messaging/Message.h"

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
    public:
        InitialLoopWaitingState(MessageDispatcher& messageDispatcher);

        void handle(std::unique_ptr<StateBase>& state, Message& message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
