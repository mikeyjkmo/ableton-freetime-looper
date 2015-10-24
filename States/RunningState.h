#pragma once
#include <string>
#include <chrono>

#include "StateBase.h"
#include "../Utilities/AsyncTimer.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Messaging/MessageDispatcher.h"

namespace AbletonProject
{

    class RunningState : public StateBase
    {
    private:
        AsyncTimer _timer;
        ConcurrentQueue<std::string> _queue;
        void forwardMessage();
        MessageDispatcher* _messageDispatcher;
    public:
        RunningState(std::chrono::milliseconds timespan, MessageDispatcher* messageDispatcher);
        virtual ~RunningState() = default;

        void dequeueAndSendAll();
        void handle(std::unique_ptr<StateBase>& state, std::string message);
    };

}
