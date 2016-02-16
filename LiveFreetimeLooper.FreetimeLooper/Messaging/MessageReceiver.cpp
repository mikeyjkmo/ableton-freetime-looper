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

    void MessageReceiver::receiveMidiMessage(
        double deltatime, std::vector<unsigned char> *rawMessage)
    {
        receiveMidiMessage(deltatime, Command(*rawMessage));
    }

    void MessageReceiver::receiveStdin(const std::string& input)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _currentState->handleStdin(_currentState, input);
    }

    bool MessageReceiver::isMidiCommand(const Command& command)
    {
        if (command.content.empty()) return false;

        auto first_byte = command.content[0];
        return first_byte > 175 && first_byte < 192;
    }

    void MessageReceiver::receiveMidiMessage(double deltatime, Command command)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!isMidiCommand(command))
            return;

        Command* correspondingStartCommand(nullptr);
        MessageType messageType(_commandMappings.getMessageType(command, correspondingStartCommand));

        if (messageType == MessageType::START)
        {
            _currentState->handle(_currentState,
                std::make_unique<StartMessage>(command, deltatime));
            return;
        }
        if (messageType == MessageType::STOP)
        {
            _currentState->handle(_currentState,
                std::make_unique<StopMessage>(command, *correspondingStartCommand, deltatime));
            return;
        }

        // todo what to do with Unknown Messages, for now, nothing
        return;
    }
}
