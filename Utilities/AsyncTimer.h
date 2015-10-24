#pragma once

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

namespace AbletonProject
{
    class AsyncTimer final
    {
    public:
        AsyncTimer(const std::chrono::milliseconds intervalMilliseconds, std::function<void()> f);
        ~AsyncTimer();

        void start();
        void stop();

    private:
        const std::chrono::milliseconds _intervalMilliseconds;
        std::function<void()> _function;

        bool _started;
        bool _stopped;
        std::shared_ptr<std::thread> _previousThread;
        std::shared_ptr<std::thread> _thread;

        void _joinThreads(bool joinCurrent);
        void _threadCallback();
        void _scheduleNextCall();

    };
}
