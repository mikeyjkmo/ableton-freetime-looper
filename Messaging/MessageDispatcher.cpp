#include "MessageDispatcher.h"

#include <iostream>

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher(RtMidiOut& mOut)
        : _midiOut(mOut)
    {
    }

    void MessageDispatcher::sendMidiMessage(Message message)
    {
        std::cout << "Sending message with payload:";
        for (auto& c : message.payload)
        {
            std::cout << " " << (unsigned int) c;
        }
        std::cout << std::endl;
        _midiOut.sendMessage(&(message.payload));
    }

}
