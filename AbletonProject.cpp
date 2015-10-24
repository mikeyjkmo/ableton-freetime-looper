// AbletonProject.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Utilities/Stopwatch.h"
#include "Utilities/ConcurrentQueue.h"
#include "Utilities/RtMidiExt.h"
#include "Messaging/MessageReceiver.h"

#include <chrono>
#include <thread>

#include "RtMidi/Rtmidi.h"

using namespace AbletonProject;

int main(int argc, char *argv[])
{
    RtMidiOut midiOut;
    unsigned int portCount = midiOut.getPortCount();

    if (argc == 1)
    {
        RtMidiExt::printPorts(midiOut, "Output");
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
        Message otherControlMessage({ 176, 7, 99 });

        // Send Messages through Created State
        receiver.receiveMessage(volumeControlMessage);
        receiver.receiveMessage(volumeControlMessage);

        std::string inputStr;
        receiver.receiveStdin(inputStr);

        // Send Message to Set Loop time to 2 seconds
        receiver.receiveMessage(volumeControlMessage);
        // Unrelated messages are just relayed
        receiver.receiveMessage(otherControlMessage);

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
