#pragma once

#include "StateBase.h"
#include "Utilities/Stopwatch.h"
#include "Messaging/MessageDispatcher.h"
#include "Messaging/Message.h"

namespace AbletonProject
{
    // Waiting for and timing completion of intial loop
    class InitialLoopState : public StateBase
    {
    private:
        Stopwatch _stopWatch;
        MessageDispatcher* _messageDispatcher;
    public:
        InitialLoopState(MessageDispatcher* messageDispatcher);
        virtual ~InitialLoopState() = default;

        void handle(std::unique_ptr<StateBase>& state, Message& message);
    };

}
