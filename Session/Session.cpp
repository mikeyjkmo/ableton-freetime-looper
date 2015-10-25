#include <iostream>
#include <chrono>
#include <thread>

#include "Utilities/Stopwatch.h"
#include "Utilities/ConcurrentQueue.h"
#include "Utilities/RtMidiExt.h"
#include "Messaging/MessageReceiver.h"
#include "Session.h"


namespace AbletonProject
{
    Session::Session(int ac, char* av[])
        : _midiOut(),
          _midiIn(),
          _numUserSpecifiedDevices(ac - 1),
          _specifiedDeviceIDs(av),
          _outputDeviceID(0),
          _inputDeviceID(0)
    {
        _setDeviceIDs();
    }

    void Session::_setDeviceIDs()
    {
        if (_numUserSpecifiedDevices > 0)
        {
            _outputDeviceID = *_specifiedDeviceIDs[1] - '0';
            if (_numUserSpecifiedDevices > 1)
            {
                _inputDeviceID = *_specifiedDeviceIDs[2] - '0';
            }
        }
    }

    bool Session::_verifyMidiPortSelections()
    {
        auto outPortCount = _midiOut.getPortCount();
        auto inPortCount = _midiIn.getPortCount();

        return (_outputDeviceID < outPortCount && _inputDeviceID < inPortCount);
    }

    int Session::_openPortsAndStartReceiving()
    {
        _midiOut.openPort(_outputDeviceID);
        _midiIn.openPort(_inputDeviceID);

        EventLogger logger;
        MessageDispatcher dispatcher(_midiOut);
        MessageReceiver receiver(dispatcher, logger);

        _midiIn.setCallback(&RtMidiExt::callbackWrapper, &receiver);
        _midiIn.ignoreTypes(false, false, false);

        while (true)
        {
            std::cout << std::endl
                      << "CONFIGURATION MODE" << std::endl
                      << "Relaying all MIDI messages for configuration. "
                      << "Press 'Enter' to enter LOOP MODE"
                      << std::endl;
            std::string input;
            std::getline(std::cin, input);
            receiver.receiveStdin(input);

            std::cout << std::endl
                      << "LOOP MODE" << std::endl
                      << "Press 'Enter' to return to CONFIGURATION MODE"
                      << std::endl;
            std::getline(std::cin, input);
            receiver.receiveStdin(input);
        }

        _midiOut.closePort();
        _midiIn.closePort();

        return 0;
    }

    int Session::_test()
    {
        _midiOut.openPort(_outputDeviceID);

        EventLogger logger;
        MessageDispatcher dispatcher(_midiOut);
        MessageReceiver receiver(dispatcher, logger);

        // Control Change: 176, 7, 100 (volume)
        Message volumeControlMessage({176, 7, 100});
        Message otherControlMessage({ 176, 7, 99 });

        // Send Messages through Created State
        receiver.receiveMidiMessage(volumeControlMessage);
        receiver.receiveMidiMessage(volumeControlMessage);

        std::string inputStr;
        receiver.receiveStdin(inputStr);

        // Send Message to Set Loop time to 2 seconds
        receiver.receiveMidiMessage(volumeControlMessage);
        // Unrelated messages are just relayed
        receiver.receiveMidiMessage(otherControlMessage);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        receiver.receiveMidiMessage(volumeControlMessage);

        // Send Messages through Running State
        for (int i = 0; i < 20; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            receiver.receiveMidiMessage(volumeControlMessage);
        }

        _midiOut.closePort();

        return 0;
    }

    void Session::_printAllAvailablePorts()
    {
        RtMidiExt::printPorts(_midiIn, "Input");
        RtMidiExt::printPorts(_midiOut, "Output");
        std::cout << std::endl;
    }

    int Session::run()
    {
        unsigned int portCount = _midiOut.getPortCount();

        if (_numUserSpecifiedDevices == 0)
        {
            _printAllAvailablePorts();
            std::cout << "Usage: AbletonProject [output_device_num] [input_device_num]"
                      << std::endl << std::endl;
            return 0;
        }

        if (portCount > 0)
        {
            if (!_verifyMidiPortSelections())
            {
                std::cout << "The MIDI device you specified does not exist" << std::endl;
                return 1;
            }

            return _openPortsAndStartReceiving();
        }
        else
        {
            std::cout << "You have no MIDI output devices on your computer" << std::endl;
        }

        return 0;
    }
}
