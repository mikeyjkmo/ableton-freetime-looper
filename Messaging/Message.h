#pragma once

#include <vector>
#include <iostream>

namespace AbletonProject
{
    class Message final
    {
    public:
        Message() {}

        Message(std::vector<unsigned char> payload) : payload(payload), deltatime(0.0)
        {
        }

        Message(double deltatime, std::vector<unsigned char> *rawMessage)
            : payload(*rawMessage), deltatime(deltatime)
        {
        }

        std::vector<unsigned char> payload;
        double deltatime;

        bool operator==(const Message& other) const
        {
            return other.payload == payload;
        }

    };
}
