#include "CreatedState.h"
#include "InitialLoopState.h"

namespace AbletonProject
{

    CreatedState::CreatedState(MessageDispatcher& messageDispatcher) :
        _messageDispatcher(messageDispatcher)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>& state, Message message)
    {
        state = std::make_unique<InitialLoopState>(_messageDispatcher);
        _messageDispatcher.sendMessage(message);
    }

}
