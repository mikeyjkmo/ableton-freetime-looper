#pragma once
#include <memory>

#include "Utilities/Stopwatch.h"
#include "Messaging/MessageDispatcher.h"
#include "StateBase.h"
#include "Messaging/Message.h"

namespace AbletonProject
{
    // Waiting for first message to start Initial Loop
    class CreatedState : public StateBase
    {
    private:
        MessageDispatcher* _messageDispatcher;
    public:
        CreatedState(MessageDispatcher* messageDispatcher);
        virtual ~CreatedState() = default;

        void handle(std::unique_ptr<StateBase>& state, Message& message);
    };

}
