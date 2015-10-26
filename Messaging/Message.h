#pragma once

#include <vector>
#include <iostream>

namespace LiveFreetimeLooper
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

namespace std
{
    template <>
    struct hash<LiveFreetimeLooper::Message>
    {
        size_t operator()(const LiveFreetimeLooper::Message& k) const
        {
            // Hash just based on payload
            std::size_t seed = 0;
            for (auto& i : k.payload)
            {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}
