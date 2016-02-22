#pragma once

#include <memory>
#include <vector>

namespace LiveFreetimeLooper
{
    class StartMessage;
    class Command;

    class ILoopTracker
    {
    protected:
        ILoopTracker() {}
    public:
        virtual ~ILoopTracker() = default;
        virtual void startCommand(Command command) = 0;
        virtual void stopCommand(const Command& correspondingStartCommand) = 0;
        virtual std::vector<Command> getNextRestartCommands() = 0;
        virtual void incrementInterval() = 0;
        virtual void clear() = 0;
    };
}
