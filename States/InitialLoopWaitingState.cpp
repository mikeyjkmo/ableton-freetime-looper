#include "InitialLoopWaitingState.h"

namespace AbletonProject
{
    InitialLoopWaitingState::InitialLoopWaitingState(StateResources& resources)
        : _resources(resources)
    {
    }

    void InitialLoopWaitingState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
        Message tempMessage = *message;
        _resources.loopTracker.commandReceived(std::move(message));
        state = std::make_unique<InitialLoopState>(_resources, tempMessage);

        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("Loop starting message detected, moving from InitialLoopWaiting to InitialLoop"),
            std::string("InitialLoopWaitingState")));
    }

    void InitialLoopWaitingState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_resources));
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn detected, reverting from InitialLoopWaiting to Created"),
            std::string("InitialLoopWaitingState")));
    }

}
