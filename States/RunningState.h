#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "StateBase.h"
#include "Utilities/AsyncTimer.h"
#include "Utilities/ConcurrentQueue.h"

namespace LiveFreetimeLooper
{

    class RunningState final : public StateBase
    {
    private:
        AsyncTimer _asyncTimer;
        ConcurrentQueue<std::unique_ptr<Message>> _queue;
        StateResources& _resources;

        void forwardMessage();
        void _dequeueAndSendAll();
    public:
        RunningState(StateResources& resources,
            std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> timespan);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, std::string& input) override;
    };

}
