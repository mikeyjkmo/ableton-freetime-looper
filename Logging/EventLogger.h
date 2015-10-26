#pragma once
#include <memory>
#include "EventBase.h"

namespace LiveFreetimeLooper
{

    class EventLogger final
    {
    public:
        EventLogger();

        void log(std::unique_ptr<EventBase> evnt);
    };
}
