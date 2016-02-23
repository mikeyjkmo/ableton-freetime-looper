#pragma once
#include <memory>
#include "IEvent.hpp"

namespace LiveFreetimeLooper
{

    class IEventLogger 
    {
    protected:
        explicit IEventLogger() {};
    public:
        virtual ~IEventLogger() = default;
        virtual void log(std::unique_ptr<IEvent> evnt) = 0;
    };
}