#pragma once
#include <chrono>

#include "StateBase.h"
#include "Utilities/AsyncTimer.h"
#include "Utilities/ConcurrentQueue.h"
#include "Messaging/MessageDispatcher.h"
#include "Messaging/Message.h"

namespace AbletonProject
{

    class RunningState : public StateBase
    {
    private:
        AsyncTimer _asyncTimer;
        ConcurrentQueue<Message> _queue;
        void forwardMessage();
        void _dequeueAndSendAll();
        MessageDispatcher* _messageDispatcher;
    public:
        RunningState(std::chrono::milliseconds timespan, MessageDispatcher* messageDispatcher);
        virtual ~RunningState();

        void handle(std::unique_ptr<StateBase>& state, Message message);
    };

}
