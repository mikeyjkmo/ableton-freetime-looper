#pragma once

#include <memory>
#include <vector>

namespace LiveFreetimeLooper
{
    class Message;

    class ILoopTracker
    {
    protected:
        ILoopTracker() {}
    public:
        virtual ~ILoopTracker() = default;
        virtual void commandReceived(std::unique_ptr<Message> message) = 0;
        virtual std::vector<Message*> getNextRestartMessages() = 0;
        virtual void incrementInterval() = 0;
        virtual void clear() = 0;
    };
}
