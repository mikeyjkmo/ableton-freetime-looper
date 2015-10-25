#pragma once

#include <iostream>
#include <chrono>
#include <memory>

#include "EventBase.h"

namespace AbletonProject
{

    class EventLogger final
    {
    public:
        EventLogger();

        void Log(std::unique_ptr<EventBase> evnt);
    };
}
