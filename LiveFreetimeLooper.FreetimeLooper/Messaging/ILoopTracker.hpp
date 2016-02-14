#pragma once

#include <memory>
#include <vector>

namespace LiveFreetimeLooper
{
    class StartMessage;

    class ILoopTracker
    {
    protected:
        ILoopTracker() {}
    public:
        virtual ~ILoopTracker() = default;
        virtual void commandReceived(std::unique_ptr<StartMessage> message) = 0;
        virtual std::vector<StartMessage*> getNextRestartMessages() = 0;
        virtual void incrementInterval() = 0;
        virtual void clear() = 0;
    };
}
