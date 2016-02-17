#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "StateBase.hpp"
#include "../Utilities/IAsyncTimer.hpp"
#include "../Utilities/IAsyncTimerFactory.hpp"
#include "../Utilities/ConcurrentQueue.hpp"

namespace LiveFreetimeLooper
{

    class RunningState final : public StateBase
    {
    private:
        std::unique_ptr<IAsyncTimer> _asyncTimer;
        ConcurrentQueue<std::unique_ptr<StartMessage>> _queue;
        StateResources& _resources;

        void _dequeueAndSendAll();
    public:
        RunningState(StateResources& resources,
            std::chrono::duration<std::chrono::high_resolution_clock::rep,
            std::chrono::high_resolution_clock::period> timespan);

        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StartMessage> message) override;
        void handle(std::unique_ptr<StateBase>& state, std::unique_ptr<StopMessage> message) override;
        void handleStdin(std::unique_ptr<StateBase>& state, const std::string& input) override;
    };

}
