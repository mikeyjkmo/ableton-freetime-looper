// AbletonProject.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Utilities/Stopwatch.h"
#include "Utilities/ConcurrentQueue.h"
#include "Messaging/MessageReceiver.h"

#include <chrono>
#include <thread>

using namespace AbletonProject;

int main(int argc, char* argv[])
{
    MessageReceiver receiver;

    receiver.receiveMessage("foo");
    receiver.receiveMessage("bar");
    receiver.receiveMessage("zim");
    receiver.receiveMessage("zam");

    return 0;
}
