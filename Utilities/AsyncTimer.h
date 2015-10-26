#pragma once

#include <functional>
#include <chrono>
#include <future>
#include <memory>

namespace LiveFreetimeLooper
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
        std::unique_ptr<std::thread> _previousThread;
        std::unique_ptr<std::thread> _currentThread;

        void _joinThreads(bool joinCurrent);
        void _threadCallback();
        void _scheduleNextCall();

    };
}
