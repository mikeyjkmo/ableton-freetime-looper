#pragma once
#include <memory>
#include <mutex>
#include <string>

#include "States/StateBase.h"
#include "States/CreatedState.h"
#include "MessageDispatcher.h"
#include "Message.h"

namespace AbletonProject
{
    class MessageReceiver final
    {
    private:
        std::unique_ptr<StateBase> _currentState;
        std::mutex _mutex;
    public:
        MessageReceiver(MessageDispatcher& messageDispatcher);

        void receiveRawMidiMessage(
            double deltatime, std::vector<unsigned char> *rawMessage, void *userData);
        void receiveMidiMessage(Message message);
        void receiveStdin(std::string& input);
    };
}
