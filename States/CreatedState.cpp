#include "CreatedState.h"

namespace AbletonProject
{
    CreatedState::CreatedState(StateResources& resources) : _resources(resources)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
    }

    void CreatedState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::make_unique<InitialLoopWaitingState>(_resources);
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, moving from Created to InitialLoopWaiting"),
            std::string("CreatedState")));
    }

}
