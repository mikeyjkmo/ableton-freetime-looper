#pragma once

#include <chrono>
#include <cstdint>
#include <cstdint>

namespace AbletonProject
{

    class Stopwatch
    {
    private:
        bool _isRunning;
        std::chrono::system_clock::time_point _startTime;
        std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> _elapsed;
        std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> calculateElapsed() const;
        std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> getElapsed() const;
    public:
        Stopwatch();
        virtual ~Stopwatch() = default;

        void start();
        void stop();
        void reset();
        void restart();
        bool IsRunning();
        std::chrono::milliseconds getElapsedMilliseconds() const;
    };
}
