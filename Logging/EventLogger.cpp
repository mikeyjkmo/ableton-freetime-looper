#include <iostream>
#include "EventLogger.h"

namespace LiveFreetimeLooper
{
    EventLogger::EventLogger()
    {
    }

    void EventLogger::log(std::unique_ptr<EventBase> evnt)
    {
        // todo time
        std::cout
            << "Name: "
            << evnt->getName()
            << "|| Level: "
            << evnt->getLogLevel()
            << "|| Detail: "
            << evnt->getDetail()
            << "|| Source: "
            << evnt->getEventSource()
            << std::endl;
    }
}
