#include <iostream>
#include "EventLogger.h"

namespace LiveFreetimeLooper
{
    EventLogger::EventLogger()
    {
    }

    void EventLogger::log(std::unique_ptr<IEvent> evnt)
    {
        std::cout
            << evnt->getName()
            << "\t: "
            << evnt->getDetail()
            << std::endl;
    }
}
