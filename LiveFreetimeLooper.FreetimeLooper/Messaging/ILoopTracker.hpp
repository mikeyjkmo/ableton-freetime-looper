#pragma once

#include <memory>
#include <vector>
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class StartMessage;

    class ILoopTracker
    {
    protected:
        ILoopTracker() {}
    public:
        virtual ~ILoopTracker() = default;
        virtual void commandReceived(Command command) = 0;
        virtual std::vector<Command> getNextRestartCommands() = 0;
        virtual void incrementInterval() = 0;
        virtual void clear() = 0;
    };
}
