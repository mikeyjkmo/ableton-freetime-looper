#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ILoopTracker.hpp"
#include "StartMessage.hpp"
#include "RunningLoop.hpp"
#include "RecordingLoop.hpp"
#include "Command.hpp"

namespace LiveFreetimeLooper
{
    class CommandMappings;

    class LoopTracker final : public ILoopTracker
    {
    private:
        std::unordered_map<Command, std::unique_ptr<RecordingLoop>> _recording;
        std::unordered_map<Command, std::unique_ptr<RunningLoop>> _running;
    public:
        LoopTracker();

        void startCommand(Command command) override;
        void stopCommand(const Command& correspondingStartCommand) override;
        std::vector<Command> getNextRestartCommands() override;
        void incrementInterval() override;
        void clear() override;
    };
}
