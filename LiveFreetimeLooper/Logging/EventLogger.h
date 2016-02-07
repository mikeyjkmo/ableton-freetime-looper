#pragma once
#include <memory>
#include "IEventLogger.h"
#include "IEvent.h"

namespace LiveFreetimeLooper
{

    class EventLogger final : public IEventLogger
    {
    public:
        EventLogger();

        void log(std::unique_ptr<IEvent> evnt);
    };
}
