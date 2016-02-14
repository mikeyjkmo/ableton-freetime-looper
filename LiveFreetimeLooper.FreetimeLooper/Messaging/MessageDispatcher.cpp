#include <string>
#include <iostream>
#include "MessageDispatcher.hpp"
#include "Message.hpp"
#include "../Logging/IEventLogger.hpp"
#include "../Logging/MessageDispatchedEvent.hpp"

namespace LiveFreetimeLooper
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut, IEventLogger& logger)
        : _midiOut(mOut),
        _logger(logger)
    {
    }

    void MessageDispatcher::sendMidiMessage(Message* message)
    {
        _logger.log(std::make_unique<MessageDispatchedEvent>(*message, std::string("MessageDispatcher")));

        _midiOut.sendMessage(&(message->payload));
    }

}
