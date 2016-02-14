#pragma once
#include <memory>
#include <string>

#include "StateBase.hpp"
#include "../Utilities/Stopwatch.hpp"
#include "../Messaging/StartMessage.hpp"

namespace LiveFreetimeLooper
{
    // Waiting for and timing completion of initial loop
    class InitialLoopState final : public StateBase
    {
    private:
        Stopwatch _stopWatch;
        StateResources& _resources;
        StartMessage _loopStartingMessage;
    public:
        InitialLoopState(StateResources& resources, StartMessage& message);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StartMessage> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, const std::string& input) override;
    };

}
