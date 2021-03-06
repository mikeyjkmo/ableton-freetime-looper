#pragma once
#include <memory>
#include "IEventLogger.hpp"
#include "IEvent.hpp"

namespace LiveFreetimeLooper
{

    class EventLogger final : public IEventLogger
    {
    public:
        explicit EventLogger();

        void log(std::unique_ptr<IEvent> evnt) override;
    };
}
