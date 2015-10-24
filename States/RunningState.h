#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "StateBase.h"
#include "Utilities/AsyncTimer.h"
#include "Utilities/ConcurrentQueue.h"
#include "Messaging/MessageDispatcher.h"
#include "Messaging/Message.h"

// Next state:
#include "CreatedState.h"

namespace AbletonProject
{

    class RunningState final : public StateBase
    {
    private:
        AsyncTimer _asyncTimer;
        ConcurrentQueue<Message> _queue;
        void forwardMessage();
        void _dequeueAndSendAll();
        MessageDispatcher& _messageDispatcher;
    public:
        RunningState(std::chrono::milliseconds timespan, MessageDispatcher& messageDispatcher);

        void handle(std::unique_ptr<StateBase>& state, Message& message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
