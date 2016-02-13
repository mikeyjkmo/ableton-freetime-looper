#include "..\Catch\catch.hpp"

#include <memory>
#include <chrono>

#include "..\Mocks\MockEventLogger.hpp"
#include "..\Mocks\MockMessageDispatcher.hpp"
#include "..\Mocks\MockAsyncTimerFactory.hpp"
#include "..\Mocks\MockAsyncTimer.hpp"
#include "..\Mocks\MockLoopTracker.hpp"

#include "..\..\LiveFreetimeLooper.FreetimeLooper\Messaging\Message.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\Messaging\LoopTracker.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\StateBase.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\CreatedState.hpp"
#include "..\..\LiveFreetimeLooper.FreetimeLooper\States\RunningState.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Running State")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<RunningState>(resources, std::chrono::high_resolution_clock::duration::zero());

    std::vector<unsigned char> messagePayload = { 1 };
    std::vector<unsigned char> messagePayloadTwo = { 2 };

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    SECTION("Running State starts the AsyncTimer")
    {
        REQUIRE(timer->isRunning());       
    }

    SECTION("Running State queues requests until the async timer recurs")
    {    
        state->handle(state, std::make_unique<Message>(messagePayload));
        state->handle(state, std::make_unique<Message>(messagePayloadTwo));
        REQUIRE(dispatcherMock.getMessages().size() == 0);

        timer->step();
        REQUIRE(dispatcherMock.getMessages().size() == 2);

        REQUIRE(dispatcherMock.getMessages()[dispatcherMock.getMessages().size() - 2] == messagePayload);
        REQUIRE(dispatcherMock.getMessages().back() == messagePayloadTwo);
    }

    SECTION("Running State ignores a loop defined by two (roughly) simultanous messages as a loop of length 1, but still relays both messages")
    {
        state->handle(state, std::make_unique<Message>(messagePayload)); 
        state->handle(state, std::make_unique<Message>(messagePayload)); 
        REQUIRE(dispatcherMock.getMessages().size() == 0);
        timer->step();  // LOOP START RECORDING + LOOP STOP RECORDING simultaneously
        REQUIRE(dispatcherMock.getMessages().size() == 2);
        timer->step();
        REQUIRE(dispatcherMock.getMessages().size() == 2);
        timer->step();
        REQUIRE(dispatcherMock.getMessages().size() == 2);
    }

    SECTION("Running State quantises a loop of length 1 correctly")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 0);
        timer->step();   // LOOP START RECORDING
        REQUIRE(dispatcherMock.getMessages().size() == 1);
        state->handle(state, std::make_unique<Message>(messagePayload));
        timer->step(); // LOOP STOP RECORDING + LOOP RESTARTED
        REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step();
        REQUIRE(dispatcherMock.getMessages().size() == 4);
    }

    SECTION("Running State quantises a loop of length 5 correctly")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 0); // Message sent at start

        timer->step(); // 1st interation finishes with queued start recording message. LOOP START SENT.
        CAPTURE(1); REQUIRE(dispatcherMock.getMessages().size() == 1);
        timer->step();
        CAPTURE(2); REQUIRE(dispatcherMock.getMessages().size() == 1);
        timer->step();
        CAPTURE(3); REQUIRE(dispatcherMock.getMessages().size() == 1);
        timer->step();
        CAPTURE(4); REQUIRE(dispatcherMock.getMessages().size() == 1);
        timer->step();
        CAPTURE(5); REQUIRE(dispatcherMock.getMessages().size() == 1);

        state->handle(state, std::make_unique<Message>(messagePayload));

        timer->step(); // 5th interation finishes with end recording message. LOOP END SENT +  LOOP RESTART SENT
        CAPTURE(6); REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step(); 
        CAPTURE(7); REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step(); 
        CAPTURE(8); REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step(); 
        CAPTURE(9); REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step();
        CAPTURE(10); REQUIRE(dispatcherMock.getMessages().size() == 3);
        timer->step(); // LOOP RESTART SENT
        CAPTURE(11); REQUIRE(dispatcherMock.getMessages().size() == 4);
        timer->step(); 
        CAPTURE(12); REQUIRE(dispatcherMock.getMessages().size() == 4); 
        timer->step();
        CAPTURE(13); REQUIRE(dispatcherMock.getMessages().size() == 4);
        timer->step();
        CAPTURE(14); REQUIRE(dispatcherMock.getMessages().size() == 4);
        timer->step();
        CAPTURE(15); REQUIRE(dispatcherMock.getMessages().size() == 4);
        timer->step(); // LOOP RESTART SENT
        CAPTURE(16); REQUIRE(dispatcherMock.getMessages().size() == 5);
        timer->step(); 
        CAPTURE(17); REQUIRE(dispatcherMock.getMessages().size() == 5);
        timer->step();
        CAPTURE(18); REQUIRE(dispatcherMock.getMessages().size() == 5);
        timer->step();
        CAPTURE(19); REQUIRE(dispatcherMock.getMessages().size() == 5);
        timer->step();
    }

    SECTION("A message is relayed, but ignored for quantising purposes, after the second occurance")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 0); // LOOP START

        timer->step(); timer->step(); timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getMessages().size() == 1);
        state->handle(state, std::make_unique<Message>(messagePayload));

        timer->step(); // LOOP END + LOOP RESTART
        REQUIRE(dispatcherMock.getMessages().size() == 3);
        state->handle(state, std::make_unique<Message>(messagePayload));
        timer->step(); // IGNORED MESSAGE
        timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getMessages().size() == 4);

        timer->step();  timer->step();
        state->handle(state, std::make_unique<Message>(messagePayload)); 
        timer->step(); // IGNORED MESSAGE
        state->handle(state, std::make_unique<Message>(messagePayload));
        timer->step(); // IGNORED MESSAGE
        timer->step();

        REQUIRE(dispatcherMock.getMessages().size() == 7);

        timer->step(); timer->step(); timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getMessages().size() == 8);
    }

    SECTION("Running State returns CreatedState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }
}

TEST_CASE("Running State (with mock looptracker)")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    MockLoopTracker loopTrackerMock;

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<RunningState>(resources, std::chrono::high_resolution_clock::duration::zero());

    std::vector<unsigned char> messagePayload = { 1 };
    std::vector<unsigned char> messagePayloadTwo = { 2 };

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    SECTION("If there are incoming messages the loop tracker is notified on every recurrance of the timer")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 0);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(*loopTrackerMock.getCommandsReceived().back() == messagePayload);
        timer->step();
        timer->step();
        state->handle(state, std::make_unique<Message>(messagePayloadTwo));
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 2);
        REQUIRE(*loopTrackerMock.getCommandsReceived().back() == messagePayloadTwo);
    }

    SECTION("The loop tracker is not notified of restarting messages, only newly received ones")
    {
        state->handle(state, std::make_unique<Message>(messagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 0);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
    }

    SECTION("The loop tracker function getNextRestartMessages once on every recurrance of the timer")
    {
        REQUIRE(loopTrackerMock.getNextRestartMessagesCallCount() == 0);
        for (int i = 1;i < 11;i++)
        {
            timer->step();
            REQUIRE(loopTrackerMock.getNextRestartMessagesCallCount() == i);
        }
    }

    SECTION("The loop tracker function incrementInterval is called once on every recurrance of the timer")
    {
        REQUIRE(loopTrackerMock.getInterval() == 0);
        for (int i = 1;i < 11;i++)
        {
            timer->step();
            REQUIRE(loopTrackerMock.getInterval() == i);
        }
    }
}

// todo MessageCancellingIdeas: tests for cancelling a loop and also the orginal loop. For cancelling and restarting.