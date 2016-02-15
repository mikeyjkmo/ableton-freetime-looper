#include "../Catch/catch.hpp"

#include <memory>
#include <chrono>
#include <thread>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"
#include "../Mocks/MockAsyncTimer.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/StartMessage.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/CommandMappings.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopWaitingState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/RunningState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Utilities/AsyncTimerFactory.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("A loop is quantised, and continue to restart whilst other loops are added")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    LoopTracker loopTracker;
    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTracker, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<CreatedState>(resources);
    auto send = [&state](std::vector<unsigned char> command) { state->handle(state, std::make_unique<StartMessage>(command)); };
    
    std::vector<unsigned char> quantisableCommand = { 0, 1 };
    std::vector<unsigned char> unrelatedCommand = { 12, 3 };

    // Send StdIn to move to InitialLoopWaitingState
    state->handleStdin(state, std::string("this is a string"));
    REQUIRE(dynamic_cast<InitialLoopWaitingState*>(state.get()));

    // Send in the first message, to move to InitialLoopState
    send(quantisableCommand);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getCommands().size() == 1);
    REQUIRE(dispatcherMock.getCommands().back().content == quantisableCommand);

    send(unrelatedCommand);

    REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    REQUIRE(dispatcherMock.getCommands().size() == 2);
    REQUIRE(dispatcherMock.getCommands().back().content == unrelatedCommand);

    // Send in the second message, to move to RunningState
    send(quantisableCommand);

    REQUIRE(dynamic_cast<RunningState*>(state.get()));
    REQUIRE(dispatcherMock.getCommands().size() == 3);
    REQUIRE(dispatcherMock.getCommands().back().content == quantisableCommand);

    auto timer = asyncTimerFactory.getCreatedTimersWeakRefs().back();

    // The quantised loop restart message is being dispached once per step 
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 4);
    REQUIRE(dispatcherMock.getCommands().back().content == quantisableCommand);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 5);
    REQUIRE(dispatcherMock.getCommands().back().content == quantisableCommand);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 6);
    REQUIRE(dispatcherMock.getCommands().back().content == quantisableCommand);

    // Introduce more loops
    // => A) Loop of length 1 (original quantised) + B) Loop of length 1 + C) Loop of length 2
    std::vector<unsigned char> loopOfLengthOneCommand = { 2, 54 };
    std::vector<unsigned char> loopOfLengthTwoCommand = { 24, 4 };

    send(loopOfLengthOneCommand);
    REQUIRE(dispatcherMock.getCommands().size() == 6);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 8);
    send(loopOfLengthTwoCommand);
    send(loopOfLengthOneCommand);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 12);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 14);
    send(loopOfLengthTwoCommand);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 18);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 20); // +2: A and B restart 
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 23); // +3: A, B and C restart
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 25);
    timer->step();
    REQUIRE(dispatcherMock.getCommands().size() == 28);
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
//     REQUIRE(dispatcherMock.getCommands().size() == 2);
//     std::this_thread::sleep_for(std::chrono::milliseconds((4 * waitMilliseconds) + 10));
// 
//     REQUIRE(dispatcherMock.getCommands().size() == 6);
// }