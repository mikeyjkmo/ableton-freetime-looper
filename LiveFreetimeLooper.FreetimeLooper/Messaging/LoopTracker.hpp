#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ILoopTracker.hpp"
#include "StartMessage.hpp"
#include "RunningLoop.hpp"
#include "RecordingLoop.hpp"

namespace LiveFreetimeLooper
{
    class CommandMappings;

    class LoopTracker final : public ILoopTracker
    {
    private:
        std::unordered_map<StartMessage, std::unique_ptr<RecordingLoop>> _recording;
        std::unordered_map<StartMessage, std::unique_ptr<RunningLoop>> _running;
        const CommandMappings& _commandMappings;
    public:
        LoopTracker(const CommandMappings& commandMappings);

        void commandReceived(std::unique_ptr<StartMessage> message);
        std::vector<StartMessage*> getNextRestartMessages();
        void incrementInterval();
        void clear();

    };
}
