#include <string>
#include <iostream>
#include "MessageDispatcher.hpp"
#include "Message.hpp"
#include "IMessage.hpp"
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
        Message message(command);
        _logger.log(std::make_unique<MessageDispatchedEvent>(message, std::string("MessageDispatcher")));

        auto content = message.getCommand().content;
        _midiOut.sendMessage(&(content));
    }

    void MessageDispatcher::sendMidiMessage(IMessage* message)
    {
        _logger.log(std::make_unique<MessageDispatchedEvent>(*message, std::string("MessageDispatcher")));

        auto content = message->getCommand().content;
        _midiOut.sendMessage(&(content)); // todo doesn't it care about delta-time?
    }
}
