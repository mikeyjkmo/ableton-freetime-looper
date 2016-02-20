#include "../Catch/catch.hpp"

#include <memory>
#include <chrono>
#include <thread>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"
#include "../Mocks/MockAsyncTimer.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/CommandMappings.hpp"
#include "../../LiveFreetimeLooper.Core/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.Core/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.Core/States/InitialLoopWaitingState.hpp"
#include "../../LiveFreetimeLooper.Core/States/InitialLoopState.hpp"
#include "../../LiveFreetimeLooper.Core/States/RunningState.hpp"
#include "../../LiveFreetimeLooper.Core/Utilities/AsyncTimerFactory.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Smoke test")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);

    auto sendStart = [&state](Command command)
    {
        state->handle(state, std::make_unique<StartMessage>(command));
    };

    auto sendStop = [&state](Command command, const Command& startCommand)
    {
        state->handle(state, std::make_unique<StopMessage>(command, startCommand));
    };

    auto numberOfDispatchedCommands = [&dispatcherMock]
    {
        return dispatcherMock.getDispatchedCommands().size();
    };

    std::vector<unsigned char> quantisableCommandContent = { 0, 1 };
    std::vector<unsigned char> unrelatedCommandContent = { 12, 3 };
    Command quantisableCommand(quantisableCommandContent);
    Command unrelatedCommand(unrelatedCommandContent);

    // Send StdIn to move to InitialLoopWaitingState
    state->handleStdin(state, std::string("this is a string"));
    REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));

    // Send in the first message, to move to InitialLoopState
    sendStart(quantisableCommand);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(numberOfDispatchedCommands() == 1);
    REQUIRE(dispatcherMock.getDispatchedCommands().back().content == quantisableCommand.content);

    sendStart(unrelatedCommand);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(numberOfDispatchedCommands() == 1);

    // Send in the second message, to move to RunningState
    sendStart(quantisableCommand);

    REQUIRE(dynamic_cast<RunningState*>(state.get()));
    REQUIRE(numberOfDispatchedCommands() == 2);
    REQUIRE(dispatcherMock.getDispatchedCommands().back().content == quantisableCommand.content);

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    // The quantised loop restart message is being dispached once per step 
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 3);
    REQUIRE(dispatcherMock.getDispatchedCommands().back().content == quantisableCommand.content);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 4);
    REQUIRE(dispatcherMock.getDispatchedCommands().back().content == quantisableCommand.content);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 5);
    REQUIRE(dispatcherMock.getDispatchedCommands().back().content == quantisableCommand.content);

    // Introduce more loops
    // => A) Loop of length 1 (original quantised) + B) Loop of length 1 + C) Loop of length 2
    std::vector<unsigned char> loopOfLengthOneCommandContent = { 2, 54 };
    std::vector<unsigned char> loopOfLengthTwoCommandContent = { 24, 4 };
    Command loopOfLengthOneCommand(loopOfLengthOneCommandContent);
    Command loopOfLengthTwoCommand(loopOfLengthTwoCommandContent);

    sendStart(loopOfLengthOneCommand);
    REQUIRE(numberOfDispatchedCommands() == 5);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 7);
    sendStart(loopOfLengthTwoCommand);
    sendStart(loopOfLengthOneCommand);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 11);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 13);
    sendStart(loopOfLengthTwoCommand);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 17);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 19); // +2: A and B restart 
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 22); // +3: A, B and C restart
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 24); // +2
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 27); // +3

    // Stop loopOfLengthOne introduced loop
    std::vector<unsigned char> stopCommandContent = { 31, 9 };
    Command stopCommand(stopCommandContent);
    sendStop(stopCommand, loopOfLengthOneCommand);
    REQUIRE(numberOfDispatchedCommands() == 28); // +1 stop message
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 29); // +1 A restarts
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 31); // +2 A and C restarts
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 32); // +1
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 34); // +2

    // Re-introduce loopOfLengthOne
    sendStart(loopOfLengthOneCommand);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 36); // +1 and B command 
    sendStart(loopOfLengthOneCommand);
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 40); // +4 A, B, C and B command
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 42); // +2 A, B
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 45); // +3 A, B, C
    timer->step();
    REQUIRE(numberOfDispatchedCommands() == 47); // +2 A, B
}

// //Uses std::this_thread::sleep_for and measures outputs based on actual time.
// //FIXME: RACE CONDITION MEANS THAT THIS TEST DOES NOT PASS ON ALL PLATFORMS
//TEST_CASE("The Loop quantising interval is correctly set")
//{
//    std::chrono::milliseconds wait(15);
//    std::chrono::milliseconds epsilon(2);
//
//    MockEventLogger loggerMock;
//    MockMessageDispatcher dispatcherMock;
//    CommandMappings commandMappings;
//    LoopTracker loopTracker(commandMappings);
//    MockAsyncTimerFactory asyncTimerFactory;
//
//    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
//    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
//    auto send = [&state](std::vector<unsigned char> command) { state->handle(state, std::make_unique<StartMessage>(command)); };
//
//    std::vector<unsigned char> quantisableCommand = { 0, 1 };
//    state->handleStdin(state, std::string("this is a string"));
//
//    // First Loop
//    send(quantisableCommand);
//    std::this_thread::sleep_for(wait);
//    send(quantisableCommand);
//
//    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();
//    auto interval = timer->getInterval();
//
//    CAPTURE(interval.count());
//    REQUIRE(interval > wait - epsilon);
//    REQUIRE(interval < wait + epsilon);
//}
//
// //FIXME: RACE CONDITION MEANS THAT THIS TEST DOES NOT PASS ON ALL PLATFORMS
// //Uses std::this_thread::sleep_for and measures outputs based on actual time.
// TEST_CASE("The Loop quantising interval is correctly set and works as expected")
// {
//     const int waitMilliseconds = 20;
//     MockEventLogger loggerMock;
//     MockMessageDispatcher dispatcherMock;
//     CommandMappings commandMappings;
//     LoopTracker loopTracker(commandMappings);
//     AsyncTimerFactory asyncTimerFactory;
// 
//     StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
//     std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
//     auto send = [&state](std::vector<unsigned char> command) { state->handle(state, std::make_unique<StartMessage>(command)); };
// 
//     std::vector<unsigned char> quantisableCommand = { 0, 1 };
//     state->handleStdin(state, std::string("this is a string"));
// 
//     // First Loop
//     send(quantisableCommand);
//     std::this_thread::sleep_for(std::chrono::milliseconds(waitMilliseconds));
//     send(quantisableCommand);
// 
//     REQUIRE(dispatcherMock.getDispatchedCommands().size() == 2);
//     std::this_thread::sleep_for(std::chrono::milliseconds((4 * waitMilliseconds) + 10));
// 
//     REQUIRE(dispatcherMock.getDispatchedCommands().size() == 6);
// }
