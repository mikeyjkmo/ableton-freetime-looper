#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver(MessageDispatcher& messageDispatcher) :
        _currentState(std::make_unique<CreatedState>(messageDispatcher))
    {
    }

    void MessageReceiver::receiveMidiMessage(Message& message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handle(_currentState, message);
    }

    void MessageReceiver::receiveStdin(std::string& input)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handleStdin(_currentState, input);
    }
}
