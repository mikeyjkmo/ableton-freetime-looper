#include <string>
#include <iostream>
#include "MessageDispatcher.hpp"
#include "StartMessage.hpp"
#include "../Logging/IEventLogger.hpp"
#include "../Logging/MessageDispatchedEvent.hpp"

namespace LiveFreetimeLooper
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut, IEventLogger& logger)
        : _midiOut(mOut),
        _logger(logger)
    {
    }

    void MessageDispatcher::sendMidiMessage(StartMessage* message)
    {
        _logger.log(std::make_unique<MessageDispatchedEvent>(*message, std::string("MessageDispatcher")));

        _midiOut.sendMessage(&(message->payload));
    }

}
