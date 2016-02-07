#pragma once
#include <memory>
#include <string>

#include "StateBase.h"
#include "../Messaging/Message.h"

namespace LiveFreetimeLooper
{
    // Waiting for stdIn to signal that intial loop start message is coming
    class CreatedState final : public StateBase
    {
    private:
        StateResources& _resources;
    public:
        CreatedState(StateResources& resources);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
