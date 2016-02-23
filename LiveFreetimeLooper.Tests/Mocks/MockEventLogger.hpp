#pragma once
#include <memory>
#include <vector>
#include "../../LiveFreetimeLooper.Core/Logging/IEventLogger.hpp"
#include "../../LiveFreetimeLooper.Core/Logging/IEvent.hpp"

class MockEventLogger final : public LiveFreetimeLooper::IEventLogger
{
private:
    std::vector<std::unique_ptr<LiveFreetimeLooper::IEvent>> _evnts;
public:
    explicit MockEventLogger();
    void log(std::unique_ptr<LiveFreetimeLooper::IEvent> evnt) override;
    std::vector<std::unique_ptr<LiveFreetimeLooper::IEvent>> const& getEvents() const;
};
