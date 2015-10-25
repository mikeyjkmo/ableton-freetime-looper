#include "CreatedState.h"

namespace AbletonProject
{

    CreatedState::CreatedState(MessageDispatcher& messageDispatcher) :
        _messageDispatcher(messageDispatcher)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>& state, Message message)
    {
        _messageDispatcher.sendMidiMessage(message);
    }

    void CreatedState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        state = std::make_unique<InitialLoopWaitingState>(_messageDispatcher);
    }

}
