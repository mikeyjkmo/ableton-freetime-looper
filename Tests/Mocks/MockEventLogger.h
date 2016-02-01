#pragma once
#include <memory>
#include <vector>
#include "Logging\IEventLogger.h"
#include "Logging\IEvent.h"

class MockEventLogger final : public LiveFreetimeLooper::IEventLogger
{
private:
    std::vector<std::unique_ptr<LiveFreetimeLooper::IEvent>> _evnts;
public:
    MockEventLogger();
    void log(std::unique_ptr<LiveFreetimeLooper::IEvent> evnt);
    std::vector<std::unique_ptr<LiveFreetimeLooper::IEvent>> const& getEvents() const;
};