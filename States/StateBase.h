#pragma once
#include <memory>

#include "Messaging/Message.h"

namespace AbletonProject
{

    class StateBase
    {
    protected:
        StateBase();
    public:
        virtual ~StateBase() = default;
        virtual void handle(std::unique_ptr<StateBase>& state, Message message) = 0;
    };

}
