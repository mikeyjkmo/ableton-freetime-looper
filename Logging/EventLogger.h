#pragma once
#include <memory>
#include "IEvent.h"

namespace LiveFreetimeLooper
{

    class EventLogger final
    {
    public:
        EventLogger();

        void log(std::unique_ptr<IEvent> evnt);
    };
}
