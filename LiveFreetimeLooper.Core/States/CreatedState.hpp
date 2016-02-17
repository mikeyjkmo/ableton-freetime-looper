#pragma once
#include <memory>
#include <string>

#include "StateBase.hpp"
#include "../Messaging/StartMessage.hpp"

namespace LiveFreetimeLooper
{
    // Waiting for stdIn to signal that intial loop start message is coming
    class CreatedState final : public StateBase
    {
    private:
        StateResources& _resources;
    public:
        CreatedState(StateResources& resources);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StartMessage> message) override;
        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StopMessage> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, const std::string& input) override;
    };

}
