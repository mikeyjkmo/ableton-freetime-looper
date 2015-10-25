#pragma once

#include <memory>
#include <unordered_map>

#include "Message.h"
#include "Loop.h"

namespace AbletonProject
{
    class LoopTracker final
    {
    private:
        std::unordered_map<Message, std::unique_ptr<Loop>> _recording;
        std::unordered_map<Message, std::unique_ptr<Loop>> _running;
    public:
        LoopTracker() {}

        void commandReceived(std::unique_ptr<Message> message);
        std::vector<Message*> getNextRestartMessages();
        void incrementInterval();
        void clear();

    };
}
