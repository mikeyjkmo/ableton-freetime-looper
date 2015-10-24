#pragma once

#include <vector>
#include <iostream>

namespace AbletonProject
{
    struct Message
    {
    public:
        Message() {}
        Message(std::vector<unsigned char> payload) :
            payload(payload)
        {
        }

        virtual ~Message() = default;

        std::vector<unsigned char> payload;
    };
}
