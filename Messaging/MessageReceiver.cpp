#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver() :
        _messageDispatcher(std::make_unique<MessageDispatcher>()),
        _currentState(std::make_unique<CreatedState>(_messageDispatcher.get()))
    {
    }

    void MessageReceiver::receiveMessage(Message& message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handle(_currentState, message);
    }

}
