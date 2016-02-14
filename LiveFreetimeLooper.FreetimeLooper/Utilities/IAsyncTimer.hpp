#pragma once

namespace LiveFreetimeLooper
{
    class IAsyncTimer
    {
    protected:
        IAsyncTimer() {};
    public:
        virtual ~IAsyncTimer() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
    };
}
