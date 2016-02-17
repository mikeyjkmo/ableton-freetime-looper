# pragma once
#include "../RtMidi/RtMidi.hpp"

namespace LiveFreetimeLooper
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

        int _openPortsAndStartReceiving();
        int _test();
        void _setDeviceIDs();
        void _printAllAvailablePorts();
        bool _verifyMidiPortSelections();
    };
}
