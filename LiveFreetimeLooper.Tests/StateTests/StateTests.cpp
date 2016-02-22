#include "../Catch/catch.hpp"

#include <memory>
#include <chrono>
#include <thread>
#include <stdexcept>

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

class SmokeTest
{
private:
    MockEventLogger _loggerMock;
    MockMessageDispatcher _dispatcherMock;
    LoopTracker _loopTracker;
    MockAsyncTimerFactory _asyncTimerFactory;

    StateResources _resources;
    std::unique_ptr<StateBase> _state;

    MockAsyncTimer* _timer;
    bool _timerHasBeenInitialised = false;

    MockAsyncTimer* getTimer()
    {
        if (!_timerHasBeenInitialised)
        {
            if (_asyncTimerFactory.getCreatedTimersWeakRefs().size() < 1)
                throw std::runtime_error("No timer has been created");

            _timerHasBeenInitialised = true;
            _timer = _asyncTimerFactory.getCreatedTimersWeakRefs().back();
        }
        return _timer;
    }
public:
    SmokeTest() :
        _loggerMock(),
        _dispatcherMock(),
        _loopTracker(),
        _asyncTimerFactory(),
        _resources(_dispatcherMock, _loopTracker, _loggerMock, _asyncTimerFactory),
        _state(std::make_unique<CreatedState>(_resources)),
        _timer(nullptr)
    {
    }

    void sendStartMessage(Command command)
    {
        _state->handle(_state, std::make_unique<StartMessage>(command));
    }

    void sendStopMessage(Command command, const Command& startCommand)
    {
        _state->handle(_state, std::make_unique<StopMessage>(command, startCommand));
    }

    void sendStdIn()
    {
       _state->handleStdin(_state, std::string("this is a string"));
    }

    size_t getNumberOfDispatchedCommands()
    {
        return _dispatcherMock.getDispatchedCommands().size();
    }

    Command getLastCommandDispatched()
    {
        return _dispatcherMock.getDispatchedCommands().back();
    }

    void stepTimer()
    {
        getTimer()->step();
    }

    template<class T>
    bool stateIsOfType()
    {
        return dynamic_cast<T*>(_state.get()) != nullptr;
    }

};

Command createCommand(unsigned char data)
{
    std::vector<unsigned char> content = { data, 1 };
    return Command(content);
}

TEST_CASE("Smoke test")
{
    SmokeTest smokeTest;
    auto REQUIRE_CorrectNumberOfMessagesDispatched = [&smokeTest](size_t number)
    {
        REQUIRE(smokeTest.getNumberOfDispatchedCommands() == number);
    };
    auto REQUIRE_LastCommand = [&smokeTest] (const Command& command)
    {
        REQUIRE(smokeTest.getLastCommandDispatched().content == command.content);
    };

    REQUIRE(smokeTest.stateIsOfType<CreatedState>());

    // Send StdIn to move to InitialLoopWaitingState
    smokeTest.sendStdIn();
    REQUIRE(smokeTest.stateIsOfType<InitialLoopWaitingState>());

    Command quantisableCommand(createCommand(1));
    Command unrelatedCommand(createCommand(2));

    // Send in the first message, to move to InitialLoopState
    smokeTest.sendStartMessage(quantisableCommand);
    REQUIRE(smokeTest.stateIsOfType<InitialLoopState>());
    REQUIRE_CorrectNumberOfMessagesDispatched(1);
    REQUIRE(smokeTest.getLastCommandDispatched().content == quantisableCommand.content);

    // Send in an unrelated message, to no effect
    smokeTest.sendStartMessage(unrelatedCommand);
    REQUIRE(smokeTest.stateIsOfType<InitialLoopState>());
    REQUIRE_CorrectNumberOfMessagesDispatched(1 + 0);

    // Send in the second message, to move to RunningState
    smokeTest.sendStartMessage(quantisableCommand);
    REQUIRE(smokeTest.stateIsOfType<RunningState>());
    REQUIRE_CorrectNumberOfMessagesDispatched(1 + 1);
    REQUIRE_LastCommand(quantisableCommand);

    // The quantised loop restart message is being dispached once per step 
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(2 + 1);
    REQUIRE_LastCommand(quantisableCommand);

    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(3 + 1);
    REQUIRE_LastCommand(quantisableCommand);

    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(4 + 1);
    REQUIRE_LastCommand(quantisableCommand);

    // Introduce more loops
    // => A) Loop of length 1 (original quantised) + B) Loop of length 1 + C) Loop of length 2
    Command loopBCommand(createCommand(3));
    Command loopCCommand(createCommand(4));

    smokeTest.sendStartMessage(loopBCommand); // Start recording B
    REQUIRE_CorrectNumberOfMessagesDispatched(5 + 0); // Nothing sent until next timer step

    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(5 + 2); // Message just sent + A restart

    smokeTest.sendStartMessage(loopCCommand); // Start recording C
    smokeTest.sendStartMessage(loopBCommand); // Stop recording B
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(7 + 4); // Messages just sent + A and B restart

    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(11 + 2); // A and B restart

    smokeTest.sendStartMessage(loopCCommand);
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(13 + 4);  // Message just sent + A, B and C restart

    // Loops A, B and C are working correctly
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(17 + 2); // A and B restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(19 + 3); // A, B and C restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(22 + 2); // A and B restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(24 + 3); // A, B and C restart

    // Stop loop B
    Command stopCommand(createCommand(5));
    smokeTest.sendStopMessage(stopCommand, loopBCommand);
    REQUIRE_CorrectNumberOfMessagesDispatched(27 + 1); // Message just sent
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(28 + 1); // A restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(29 + 2); // A and C restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(31 + 1); // A restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(32 + 2); // A and C restart

    // Re-introduce loop B
    smokeTest.sendStartMessage(loopBCommand);
    REQUIRE_CorrectNumberOfMessagesDispatched(34 + 0);
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(34 + 2); // Message just sent and A restart
    smokeTest.sendStartMessage(loopBCommand);
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(36 + 4);  // Message just sent, A, B and C restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(40 + 2);  // A and B restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(42 + 3);  // A, B and C restart
    smokeTest.stepTimer();
    REQUIRE_CorrectNumberOfMessagesDispatched(45 + 2);  // A and B restart
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
