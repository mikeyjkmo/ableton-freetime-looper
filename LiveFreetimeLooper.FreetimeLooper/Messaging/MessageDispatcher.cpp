#include <string>
#include <iostream>
#include "MessageDispatcher.hpp"
#include "StartMessage.hpp"
#include "Command.hpp"
#include "../Logging/IEventLogger.hpp"
#include "../Logging/MessageDispatchedEvent.hpp"

namespace LiveFreetimeLooper
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut, IEventLogger& logger)
        : _midiOut(mOut),
        _logger(logger)
    {
    }

    void MessageDispatcher::sendNewMidiMessage(Command command)
    {
        StartMessage message(command);
        _logger.log(std::make_unique<MessageDispatchedEvent>(message, std::string("MessageDispatcher")));

        _midiOut.sendMessage(&(message.command.content));
    }

    void MessageDispatcher::sendMidiMessage(StartMessage* message)
    {
        _logger.log(std::make_unique<MessageDispatchedEvent>(*message, std::string("MessageDispatcher")));

        _midiOut.sendMessage(&(message->command.content)); // todo doesn't it care about delta-time?
    }
}
