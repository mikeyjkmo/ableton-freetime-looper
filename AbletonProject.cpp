// AbletonProject.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Stopwatch.h"
#include "ConcurrentQueue.h"
#include "MessageReceiver.h"

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