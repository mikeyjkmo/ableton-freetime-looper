# pragma once

class RtMidi;

namespace LiveFreetimeLooper
{
    namespace RtMidiExt
    {
        void printPorts(RtMidi& midi, const std::string &portType);
        void callbackWrapper(
            double deltatime, std::vector<unsigned char> *rawMessage, void *userData);
    }
}
