#include "MessageDispatcher.h"

#include <iostream>

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher(std::unique_ptr<RtMidiOut> mOut)
        : _midiOut(std::move(mOut))
    {
    }

    void MessageDispatcher::sendMessage(Message& message)
    {
        for (const unsigned char &i : message.payload)
            std::cout << i << std::endl;
    }

}
