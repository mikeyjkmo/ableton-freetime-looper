#include "MessageReceiver.hpp"
#include "CommandMappings.hpp"
#include "../States/CreatedState.hpp"

namespace LiveFreetimeLooper
{
    class IEventLogger;
    class IAsyncTimerFactory;
    class ILoopTracker;
    class IMessageDispatcher;

    MessageReceiver::MessageReceiver(
        IMessageDispatcher& messageDispatcher, ILoopTracker& loopTracker,
        IEventLogger& logger, IAsyncTimerFactory& asyncTimerFactory, CommandMappings& commandMappings) : 
        _resources(messageDispatcher, loopTracker, logger, asyncTimerFactory),
        _commandMappings(commandMappings),
        _currentState(std::make_unique<CreatedState>(_resources))
    {
    }

    void MessageReceiver::receiveRawMidiMessage(
        double deltatime, std::vector<unsigned char> *rawMessage)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!isMidiCommand(rawMessage))
        {
            return;
        }

        _currentState->handle(_currentState, std::make_unique<StartMessage>(rawMessage, deltatime));
    }

    void MessageReceiver::receiveStdin(const std::string& input)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handleStdin(_currentState, input);
    }

    bool MessageReceiver::isMidiCommand(std::vector<unsigned char>* rawMessage)
    {
        if (rawMessage->empty()) return false;

        auto first_byte = (*rawMessage)[0];
        return first_byte > 175 && first_byte < 192;
    }
}
