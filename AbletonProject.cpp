// AbletonProject.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Utilities/Stopwatch.h"
#include "Utilities/ConcurrentQueue.h"
#include "Messaging/MessageReceiver.h"

#include <chrono>
#include <thread>

#include "RtMidi/Rtmidi.h"

using namespace AbletonProject;

void printPorts(RtMidi& midi, const std::string &portType)
{
    // Check inputs.
    unsigned int nPorts = midi.getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI " << portType << " ports available." << std::endl;
    std::string portName;

    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            portName = midi.getPortName(i);
        }
        catch (RtMidiError &error) {
            error.printMessage();
        }
        std::cout << "  " << portType << " Port #" << i+1 << ": " << portName << std::endl;
    }
}

int main(int argc, char *argv[])
{
    RtMidiOut midiOut;
    unsigned int portCount = midiOut.getPortCount();

    if (argc == 1)
    {
        printPorts(midiOut, "Output");
        return 0;
    }

    if (portCount > 0)
    {
        // Open the specified MIDI port
        unsigned int portNum = *argv[1] - '0';

        if (portNum >= portCount)
        {
            std::cout << "The MIDI device you specified does not exist" << std::endl;
            return 1;
        }

        midiOut.openPort(portNum);

        MessageDispatcher dispatcher(midiOut);
        MessageReceiver receiver(dispatcher);

        // Control Change: 176, 7, 100 (volume)
        Message volumeControlMessage({176, 7, 100});

        // Send Messages through Created State
        receiver.receiveMessage(volumeControlMessage);
        receiver.receiveMessage(volumeControlMessage);

        std::string inputStr;
        receiver.receiveStdin(inputStr);

        // Send Message to Set Loop time to 2 seconds
        receiver.receiveMessage(volumeControlMessage);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        receiver.receiveMessage(volumeControlMessage);

        // Send Messages through Running State
        for (int i = 0; i < 20; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            receiver.receiveMessage(volumeControlMessage);
        }

        midiOut.closePort();
    }
    else
    {
        std::cout << "You have no MIDI output devices on your computer" << std::endl;
    }

    return 0;
}
