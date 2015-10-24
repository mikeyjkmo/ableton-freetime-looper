#include "MessageDispatcher.h"

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher(std::unique_ptr<RtMidiOut> mOut)
        : _midiOut(std::move(mOut))
    {
    }

    void MessageDispatcher::sendMessage(Message message)
    {
        // todo
    }

}
