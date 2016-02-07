#pragma once

#include "IAsyncTimer.h"

#include <functional>
#include <chrono>
#include <future>
#include <memory>

namespace LiveFreetimeLooper
{
    class AsyncTimer final : public IAsyncTimer
    {
    private:
        const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> _interval;
        std::function<void()> _function;

        bool _started;
        bool _stopped;
        std::unique_ptr<std::thread> _previousThread;
        std::unique_ptr<std::thread> _currentThread;

        void _joinThreads(bool joinCurrent);
        void _threadCallback();
        void _scheduleNextCall();

    public:
        AsyncTimer(const std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> interval,
            std::function<void()> func);
        ~AsyncTimer();

        void start();
        void stop();
    };
}
