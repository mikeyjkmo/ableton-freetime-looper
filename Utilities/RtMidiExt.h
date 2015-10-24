# pragma once
#include <iostream>
#include "RtMidi/Rtmidi.h"

namespace AbletonProject
{
    namespace RtMidiExt
    {
        void printPorts(RtMidi& midi, const std::string &portType);
    }
}
