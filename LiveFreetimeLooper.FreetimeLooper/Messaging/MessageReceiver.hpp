#pragma once
#include <memory>
#include <mutex>
#include <string>

#include "../States/StateBase.hpp"
#include "StartMessage.hpp"

namespace LiveFreetimeLooper
{
    class IMessageDispatcher;
    class IEventLogger;
    class ILoopTracker;
    class IAsyncTimerFactory;
    class CommandMappings;

    class MessageReceiver final
    {
    private:
        std::mutex _mutex;
        StateResources _resources;
        CommandMappings& _commandMappings;
        std::unique_ptr<StateBase> _currentState;

        bool isMidiCommand(std::vector<unsigned char>* rawMessage);
    public:
        MessageReceiver(
            IMessageDispatcher& messageDispatcher, ILoopTracker& loopTracker,
            IEventLogger& logger, IAsyncTimerFactory& asyncTimerFactory, CommandMappings& commandMappings);

        void receiveRawMidiMessage(double deltatime, std::vector<unsigned char> *rawMessage);
        void receiveStdin(const std::string& input);
    };
}
