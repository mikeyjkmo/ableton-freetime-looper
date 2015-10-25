#include "InitialLoopWaitingState.h"

namespace AbletonProject
{

    InitialLoopWaitingState::InitialLoopWaitingState(MessageDispatcher& messageDispatcher) :
        _messageDispatcher(messageDispatcher)
    {
    }

    void InitialLoopWaitingState::handle(std::unique_ptr<StateBase>& state, Message& message)
    {
        state = std::make_unique<InitialLoopState>(_messageDispatcher, message);
        _messageDispatcher.sendMidiMessage(message);
    }

    void InitialLoopWaitingState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::move(std::make_unique<CreatedState>(_messageDispatcher));
    }

}
