#pragma once

#include <algorithm>
#include <vector>

namespace LiveFreetimeLooper
{
    class Command final
    {
    public:
        // The first n items in Command::content that identify the Command
        const static int COMMAND_ID_SIZE = 2;

        Command(const std::vector<unsigned char>& content) : content(content)
        {
            if (content.size() < 2)
            {
                throw std::runtime_error(
                    "A Command must have a minimum of two elements in its contents");
            }
        }

        Command(unsigned char b1, unsigned char b2, unsigned char b3) :
            content({b1, b2, b3})
        {
        }

        std::vector<unsigned char> content;

        bool operator==(const Command& other) const
        {
            return other.content[0] == content[0] && other.content[1] == content[1];
        }

        bool operator!=(const Command& other) const
        {
            return !operator==(other);
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
            for (int i = 0; i < LiveFreetimeLooper::Command::COMMAND_ID_SIZE; ++i)
            {
                seed ^= k.content[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}
