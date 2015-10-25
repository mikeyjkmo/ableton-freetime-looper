#pragma once

#include <memory>
#include <cstdint>

#include "Message.h"

namespace AbletonProject
{

    class Loop
    {
    private:
        std::int32_t interval;
        std::unique_ptr<Message> controlMessage;
    public:
        Loop() {}
    };

}