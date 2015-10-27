#pragma once
#include <memory>
#include <string>

#include "StateBase.h"
#include "Utilities/Stopwatch.h"
#include "Messaging/Message.h"

namespace LiveFreetimeLooper
{
    // Waiting for and timing completion of initial loop
    class InitialLoopState final : public StateBase
    {
    private:
        Stopwatch _stopWatch;
        StateResources& _resources;
        Message _loopStartingMessage;
    public:
        InitialLoopState(StateResources& resources, Message& message);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
