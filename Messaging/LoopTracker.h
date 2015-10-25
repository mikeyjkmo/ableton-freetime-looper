#pragma once

#include <vector>
#include <memory>

#include "Message.h"
#include "Loop.h"

namespace AbletonProject
{
    class LoopTracker final
    {
    private:
        // todo hashmap
        std::vector<Loop> _recording;
        std::vector<Loop> _running;
    public:
        LoopTracker() {}

        void incrementInterval()
        {
            // ++ interval on all recording loops
        }

        void commandReceived(std::unique_ptr<Message> message)
        {
            // if in recording, move to running
            // otherwise ignore
        }

        std::vector<Message*> getNextRestartMessages() { return std::vector<Message*>();  }

    };
}
