#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver(MessageDispatcher& messageDispatcher, EventLogger& logger) :
        _currentState(std::make_unique<CreatedState>(messageDispatcher, logger)),
        _logger(logger)
    {
    }

    void MessageReceiver::receiveRawMidiMessage(
        double deltatime, std::vector<unsigned char> *rawMessage)
    {
        Message message(deltatime, rawMessage);
        receiveMidiMessage(message);
    }

    void MessageReceiver::receiveMidiMessage(Message message)
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
