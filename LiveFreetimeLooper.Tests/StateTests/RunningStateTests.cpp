#include "../Catch/catch.hpp"

#include <memory>
#include <chrono>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"
#include "../Mocks/MockAsyncTimer.hpp"
#include "../Mocks/MockLoopTracker.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/CommandMappings.hpp"
#include "../../LiveFreetimeLooper.Core/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.Core/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.Core/States/RunningState.hpp"

using namespace LiveFreetimeLooper;
using namespace std::chrono_literals;

TEST_CASE("Running State")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;

    LoopTracker loopTracker;
    
    MockAsyncTimerFactory asyncTimerFactory;

    auto intervalDuration = 1min;
    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<RunningState>(resources, 1min);

    std::vector<unsigned char> commandContent = { 1, 1 };
    std::vector<unsigned char> commandTwoContent = { 2, 1 };
    std::vector<unsigned char> stopCommandContent = { 3, 1 };
    Command command(commandContent);
    Command commandTwo(commandTwoContent);
    Command stopCommand(stopCommandContent);

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    SECTION("AsyncTimers is created with the correct duration and started")
    {
        REQUIRE(timer->getInterval() == intervalDuration);
        REQUIRE(timer->isRunning());
    }

    SECTION("Running State queues requests until the async timer recurs")
    {    
        state->handle(state, std::make_unique<StartMessage>(command));
        state->handle(state, std::make_unique<StartMessage>(commandTwo));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0);

        timer->step();
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);

        REQUIRE(dispatcherMock.getDispatchedCommands()[dispatcherMock.getDispatchedCommands().size() - 2].content == command.content);
        REQUIRE(dispatcherMock.getDispatchedCommands().back().content == commandTwo.content);
    }

    SECTION("Running State ignores a loop defined by two (roughly) simultanous messages as a loop of length 1, but still relays both messages")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0);
        timer->step();  // LOOP START RECORDING + LOOP STOP RECORDING simultaneously
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
        timer->step();
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
        timer->step();
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
    }

    SECTION("Running State quantises a loop of length 1 correctly")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0);
        timer->step();   // LOOP START RECORDING
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(); // LOOP STOP RECORDING + LOOP RESTARTED
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step();
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
    }

    SECTION("Running State quantises a loop of length 5 correctly")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0); // Message sent at start

        timer->step(); // 1st interation finishes with queued start recording message. LOOP START SENT.
        CAPTURE(1); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        timer->step();
        CAPTURE(2); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        timer->step();
        CAPTURE(3); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        timer->step();
        CAPTURE(4); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        timer->step();
        CAPTURE(5); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);

        state->handle(state, std::make_unique<StartMessage>(command));

        timer->step(); // 5th interation finishes with end recording message. LOOP END SENT +  LOOP RESTART SENT
        CAPTURE(6); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step(); 
        CAPTURE(7); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step(); 
        CAPTURE(8); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step(); 
        CAPTURE(9); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step();
        CAPTURE(10); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        timer->step(); // LOOP RESTART SENT
        CAPTURE(11); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
        timer->step(); 
        CAPTURE(12); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
        timer->step();
        CAPTURE(13); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
        timer->step();
        CAPTURE(14); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
        timer->step();
        CAPTURE(15); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
        timer->step(); // LOOP RESTART SENT
        CAPTURE(16); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
        timer->step(); 
        CAPTURE(17); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
        timer->step();
        CAPTURE(18); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
        timer->step();
        CAPTURE(19); REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
        timer->step();
    }

    SECTION("A message is relayed, but ignored for quantising purposes, after the second occurance")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 0); // LOOP START

        timer->step(); timer->step(); timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        state->handle(state, std::make_unique<StartMessage>(command));

        timer->step(); // LOOP END + LOOP RESTART
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 3);
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(); // IGNORED MESSAGE
        timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);

        timer->step();  timer->step();
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(); // IGNORED MESSAGE
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(); // IGNORED MESSAGE
        timer->step();

        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 7);

        timer->step(); timer->step(); timer->step(); timer->step(); timer->step();

        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 8);
    }

    SECTION("When Stdin is supplied it clears looptracker and returns state to CreatedState")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION("Running State immediately relays a Stop Message")
    {
        for (unsigned char i = 1; i < 21; i++)
        {
            std::vector<unsigned char> commandContent = { 0, i };
            std::vector<unsigned char> startCommandContent = { 1, i };
            Command startCommand(commandContent);
            Command stopCommand(startCommandContent);

            state->handle(state, std::make_unique<StopMessage>(command, startCommand));
            REQUIRE(dispatcherMock.getDispatchedCommands().size() == i);
            REQUIRE(dispatcherMock.getDispatchedCommands().back().content == command.content);
            timer->step();
            REQUIRE(dispatcherMock.getDispatchedCommands().size() == i);
            REQUIRE(dynamic_cast<RunningState*>(state.get()));
        }
    }

    // todo, "and forgotten about" this isn't the way ableton works
    SECTION("A running loop is stoped correctly and forgotten about"
        "and the the stop message is relayed")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step();
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(2);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);

        state->handle(state, std::make_unique<StopMessage>(stopCommand, command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
        timer->step(2);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);

        // Clean Slate
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(5);
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(5);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 8);
    }

    // todo, "and forgotten about" this isn't the way ableton works
    SECTION("A recording loop is stoped correctly and forgotten about"
        "and the the stop message is relayed")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step();

        state->handle(state, std::make_unique<StopMessage>(stopCommand, command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
        timer->step(10);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);

        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(4);
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(4);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 5);
    }

    // todo, "and does not affect future loop" this isn't the way ableton works
    SECTION("Try to stop an unknown loop does nothing and does not affect future loop"
        "and the the stop message is relayed")
    {
        state->handle(state, std::make_unique<StopMessage>(stopCommand, command));
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 1);
        timer->step(2);

        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(3);
        state->handle(state, std::make_unique<StartMessage>(command));
        timer->step(3);
        REQUIRE(dispatcherMock.getDispatchedCommands().size() == 4);
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

    std::vector<unsigned char> command = { 1, 1 };
    std::vector<unsigned char> commandTwo = { 2, 1 };

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    SECTION("If there are incoming messages the loop tracker is notified on every recurrance of the timer")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 0);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsStarted().back().content == command);
        timer->step();
        timer->step();
        state->handle(state, std::make_unique<StartMessage>(commandTwo));
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 2);
        REQUIRE(loopTrackerMock.getCommandsStarted().back().content == commandTwo);
    }

    SECTION("The loop tracker is not notified of restarting messages, only newly received ones")
    {
        state->handle(state, std::make_unique<StartMessage>(command));
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 0);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
        timer->step();
        REQUIRE(loopTrackerMock.getCommandsStarted().size() == 1);
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
