#include "MockEventLogger.hpp"

MockEventLogger::MockEventLogger() :
    _evnts()
{
}

void MockEventLogger::log(std::unique_ptr<LiveFreetimeLooper::IEvent> evnt)
{
    _evnts.push_back(std::move(evnt));
}

std::vector<std::unique_ptr<LiveFreetimeLooper::IEvent>>  const& MockEventLogger::getEvents() const
{
    return _evnts;
}