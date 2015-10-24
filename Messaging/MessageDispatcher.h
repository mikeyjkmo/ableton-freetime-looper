#pragma once

#include "Message.h"

namespace AbletonProject
{
    class MessageDispatcher
    {
    public:
        MessageDispatcher();
        virtual ~MessageDispatcher() = default;
        void sendMessage(Message& message);
    };
}
