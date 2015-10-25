#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "StateBase.h"
#include "Utilities/AsyncTimer.h"
#include "Utilities/ConcurrentQueue.h"
#include "Messaging/MessageDispatcher.h"
#include "Messaging/Message.h"
#include "Logging/EventLogger.h"
#include "Logging/StateChangedEvent.h"

// Next state:
#include "CreatedState.h"

namespace AbletonProject
{

    class RunningState final : public StateBase
    {
    private:
        AsyncTimer _asyncTimer;
        ConcurrentQueue<std::unique_ptr<Message>> _queue;
        void forwardMessage();
        void _dequeueAndSendAll();
        MessageDispatcher& _messageDispatcher;
        EventLogger& _logger;
    public:
        RunningState(std::chrono::milliseconds timespan, MessageDispatcher& messageDispatcher, EventLogger& logger);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
