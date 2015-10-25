#pragma once

#include <vector>
#include <memory>

#include "Message.h"

namespace AbletonProject
{
    class LoopTracker final
    {
    public:
        LoopTracker() {}

        void commandReceived(std::unique_ptr<Message> message) { }

        std::vector<Message*> getNextRestartMessages() { return std::vector<Message*>();  }

    };
}
