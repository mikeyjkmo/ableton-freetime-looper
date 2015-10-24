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

int main()
{
    MessageDispatcher dispatcher(std::make_unique<RtMidiOut>());
    MessageReceiver receiver(dispatcher);

    receiver.receiveMessage(Message());

    std::this_thread::sleep_for(std::chrono::seconds(2));

    receiver.receiveMessage(Message());

    for (int i = 0; i < 20; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        receiver.receiveMessage(Message());
    }


    return 0;
}
