#pragma once
#include <memory>
#include <mutex>
#include <string>

#include "States/StateBase.h"
#include "States/CreatedState.h"
#include "MessageDispatcher.h"
#include "Message.h"
#include "LoopTracker.h"
#include "Logging/EventLogger.h"

namespace AbletonProject
{
    class MessageReceiver final
    {
    private:
        std::unique_ptr<StateBase> _currentState;
        std::mutex _mutex;
        LoopTracker& _loopTracker;
        EventLogger _logger;

        bool isMidiCommand(Message* message);
    public:
        MessageReceiver(
            MessageDispatcher& messageDispatcher, LoopTracker& loopTracker, EventLogger& logger);

        void receiveRawMidiMessage(double deltatime, std::vector<unsigned char> *rawMessage);
        void receiveMidiMessage(std::unique_ptr<Message> message);
        void receiveStdin(std::string& input);
    };
}
