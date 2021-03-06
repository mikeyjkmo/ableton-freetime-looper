#include <iostream>
#include "../Messaging/MessageReceiver.hpp"
#include "RtMidiExt.hpp"
#include "../RtMidi/Rtmidi.hpp"

namespace LiveFreetimeLooper
{
    namespace RtMidiExt
    {
        void printPorts(RtMidi& midi, const std::string &portType)
        {
            unsigned int nPorts = midi.getPortCount();
            std::cout << std::endl << "There are " << nPorts << " MIDI "
                      << portType << " ports available." << std::endl;
            std::string portName;

            for (unsigned int i = 0; i < nPorts; ++i)
            {
                try {
                    portName = midi.getPortName(i);
                }
                catch (RtMidiError &error) {
                    error.printMessage();
                }
                std::cout << "  " << portType << " Port #"
                          << i << ": " << portName << std::endl;
            }
        }

        void callbackWrapper(
            double deltatime, std::vector<unsigned char> *rawMessage, void *userData)
        {
            (static_cast<MessageReceiver*>(userData))->receiveMidiMessage(
                    deltatime, rawMessage);
        }
    }
}
