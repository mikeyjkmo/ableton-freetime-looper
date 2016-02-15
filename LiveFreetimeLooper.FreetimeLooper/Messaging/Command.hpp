#pragma once

#include <vector>

namespace LiveFreetimeLooper
{
    class Command final
    {
    public:
        Command(std::vector<unsigned char> content) :
            content(content)
        {
        }

        std::vector<unsigned char> content;

        bool operator==(const Command& other) const
        {
            return other.content == content;
        }
    };
}

namespace std
{
    template <>
    struct hash<LiveFreetimeLooper::Command>
    {
        size_t operator()(const LiveFreetimeLooper::Command& k) const
        {
            // Hash just based on command content
            std::size_t seed = 0;
            for (auto& i : k.content)
            {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}