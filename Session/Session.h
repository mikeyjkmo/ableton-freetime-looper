# pragma once
#include "RtMidi/RtMidi.h"

namespace AbletonProject
{
    class Session final
    {
    public:
        Session(int ac, char* av[]);

        int run();
    private:
        RtMidiOut _midiOut;
        RtMidiIn _midiIn;
        int _numUserSpecifiedDevices;
        char** _specifiedDeviceIDs;
        unsigned int _outputDeviceID;
        unsigned int _inputDeviceID;

        int _test();
        void _setDeviceIDs();
        void _printAllAvailablePorts();
    };
}
