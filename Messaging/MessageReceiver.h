#pragma once
#include <memory>
#include <mutex>
#include <string>

#include "States/StateBase.h"
#include "Message.h"

namespace LiveFreetimeLooper
{
    class IMessageDispatcher;
    class IEventLogger;
    class ILoopTracker;

    class MessageReceiver final
    {
    private:
        std::mutex _mutex;
        StateResources _resources;
        std::unique_ptr<StateBase> _currentState;

        bool isMidiCommand(Message* message);
    public:
        MessageReceiver(
            IMessageDispatcher& messageDispatcher, ILoopTracker& loopTracker, IEventLogger& logger);

        void receiveRawMidiMessage(double deltatime, std::vector<unsigned char> *rawMessage);
        void receiveMidiMessage(std::unique_ptr<Message> message);
        void receiveStdin(std::string& input);
    };
}
