# pragma once
#include <iostream>
#include "RtMidi/Rtmidi.h"

namespace AbletonProject
{
    namespace RtMidiExt
    {
        void printPorts(RtMidi& midi, const std::string &portType);
        void callbackWrapper(
            double deltatime, std::vector<unsigned char> *rawMessage, void *userData);
    }
}
