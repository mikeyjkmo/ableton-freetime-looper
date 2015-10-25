# pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "RtMidi/RtMidi.h"
#include "Utilities/Stopwatch.h"
#include "Utilities/ConcurrentQueue.h"
#include "Utilities/RtMidiExt.h"
#include "Messaging/MessageReceiver.h"
#include "Messaging/MessageReceiver.h"
#include "Logging/EventLogger.h"

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

        int _openPortsAndStartReceiving();
        int _test();
        void _setDeviceIDs();
        void _printAllAvailablePorts();
        bool _verifyMidiPortSelections();
    };
}
