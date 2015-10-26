#include <string>
#include <iostream>
#include "MessageDispatcher.h"
#include "Message.h"

namespace LiveFreetimeLooper
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut)
        : _midiOut(mOut)
    {
    }

    void MessageDispatcher::sendMidiMessage(Message* message)
    {
        std::cout << "Sending message with payload:";
        for (auto& c : message->payload)
        {
            std::cout << " " << (unsigned int) c;
        }
        std::cout << std::endl;
        _midiOut.sendMessage(&(message->payload));
    }

}
