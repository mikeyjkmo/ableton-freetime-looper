#pragma once

#include <vector>

namespace LiveFreetimeLooper
{
    class StopMessage final
    {
    public:
        StopMessage(std::vector<unsigned char> payload,
            const std::vector<unsigned char>& correspondingStartPayload) :
            payload(payload),
            correspondingStartPayload(correspondingStartPayload),
            deltatime(0.0)
        {
        }

        StopMessage(std::vector<unsigned char> *rawMessage,
            const std::vector<unsigned char>& correspondingStartPayload,
            double deltatime) :
            payload(*rawMessage),
            correspondingStartPayload(correspondingStartPayload),
            deltatime(deltatime)
        {
        }

        std::vector<unsigned char> payload;
        const std::vector<unsigned char>& correspondingStartPayload;
        double deltatime;

        bool operator==(const StopMessage& other) const
        {
            return other.payload == payload;
        }
    };
}