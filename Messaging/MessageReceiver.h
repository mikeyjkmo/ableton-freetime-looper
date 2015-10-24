#pragma once
#include <memory>
#include <mutex>

#include "States/StateBase.h"
#include "States/CreatedState.h"
#include "MessageDispatcher.h"
#include "Message.h"

namespace AbletonProject
{
    class MessageReceiver
    {
    private:
        std::unique_ptr<StateBase> _currentState;
        std::mutex _mutex;
    public:
        MessageReceiver(MessageDispatcher& messageDispatcher);
        virtual ~MessageReceiver() = default;

        void receiveMessage(Message message);
    };
}
