#include "EventLogger.h"

namespace AbletonProject
{
    EventLogger::EventLogger()
    {
    }

    void EventLogger::Log(std::unique_ptr<EventBase> evnt)
    {
        // todo time
        std::cout
            << " Name: "
            << evnt->getName()
            << " Level: "
            << evnt->getLogLevel()
            << " Detail: "
            << evnt->getDetail()
            << " Source: "
            << evnt->getEventSource()
            << std::endl;
    }
}
