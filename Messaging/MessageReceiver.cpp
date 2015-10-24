#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver() :
        _currentState(std::make_unique<CreatedState>(_messageDispatcher.get()))
    {
    }

    void MessageReceiver::receiveMessage(std::string message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handle(_currentState, message);
    }

}
