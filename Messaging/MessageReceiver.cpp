#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver(MessageDispatcher& messageDispatcher) :
        _currentState(std::make_unique<CreatedState>(messageDispatcher))
    {
    }

    void MessageReceiver::receiveMessage(Message& message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handle(_currentState, message);
    }

}
