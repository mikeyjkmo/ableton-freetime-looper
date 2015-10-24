#pragma once
#include <memory>
#include <string>

#include "Messaging/Message.h"

namespace AbletonProject
{

    class StateBase
    {
    protected:
        StateBase() {};
    public:
        virtual ~StateBase() = default;
        virtual void handle(std::unique_ptr<StateBase>& state, Message& message) = 0;
        virtual void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) = 0;
    };

}
