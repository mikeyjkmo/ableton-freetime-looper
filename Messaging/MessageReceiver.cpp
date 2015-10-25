#include "MessageReceiver.h"

namespace AbletonProject
{

    MessageReceiver::MessageReceiver(
            MessageDispatcher& messageDispatcher, LoopTracker& loopTracker, EventLogger& logger)
        : _currentState(std::make_unique<CreatedState>(messageDispatcher, logger)),
          _loopTracker(loopTracker)
    {
    }

    void MessageReceiver::receiveRawMidiMessage(
        double deltatime, std::vector<unsigned char> *rawMessage)
    {
        receiveMidiMessage(std::make_unique<Message>(deltatime, rawMessage));
    }

    void MessageReceiver::receiveMidiMessage(std::unique_ptr<Message> message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (isMidiCommand(message.get()))
        {
            _currentState->handle(_currentState, std::move(message));
        }
    }

    void MessageReceiver::receiveStdin(std::string& input)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handleStdin(_currentState, input);
    }

    bool MessageReceiver::isMidiCommand(Message* message)
    {
        if (message->payload.empty()) return false;

        auto first_byte = message->payload[0];
        return first_byte > 175 && first_byte < 192;
    }
}
