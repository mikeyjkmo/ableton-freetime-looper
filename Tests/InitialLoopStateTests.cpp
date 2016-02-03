#include "Tests\Catch\catch.h"

#include <cstdint>
#include <memory>

#include "Mocks\MockEventLogger.h"
#include "Mocks\MockMessageDispatcher.h"
#include "Mocks\MockLoopTracker.h"
#include "Mocks\MockAsyncTimerFactory.h"

#include "Messaging\Message.h"
#include "States\StateBase.h"
#include "States\CreatedState.h"
#include "States\InitialLoopState.h"
#include "States\RunningState.h"

using namespace LiveFreetimeLooper;

TEST_CASE("InitialLoopState")
{
    MockEventLogger loggerMock;
    MockMessageDispatcher dispatcherMock;
    MockLoopTracker loopTrackerMock;

    std::vector<unsigned char> startingMessagePayload = { 7, 1 };
    std::vector<unsigned char> otherMessagePayload = { 3, 5 };

    MockAsyncTimerFactory asyncTimerFactory;

    StateResources resources(dispatcherMock, loopTrackerMock, loggerMock, asyncTimerFactory);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopState>(resources, Message(startingMessagePayload));

    SECTION("When InitialLoopState receives the a message with the same payload as the starting message it relays the message to the dispatcher")
    {
        state->handle(state, std::make_unique<Message>(startingMessagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 1);
        REQUIRE(dispatcherMock.getMessages().back() == startingMessagePayload);
    }

    SECTION("When InitialLoopState receives the a message with the same payload as the starting message it is relayed to the looptracker")
    {
        state->handle(state, std::make_unique<Message>(startingMessagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(*loopTrackerMock.getCommandsReceived().back() == startingMessagePayload);
    }

    SECTION("When InitialLoopState receives the a message with the same payload as the starting message the state changes to RunningState")
    {
        state->handle(state, std::make_unique<Message>(startingMessagePayload));
        REQUIRE(dynamic_cast<RunningState*>(state.get()));
    }

    SECTION("When InitialLoopState receives the a message with a different payload as the starting message it is relayed to the dispatcher")
    {
        state->handle(state, std::make_unique<Message>(otherMessagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 1);
        REQUIRE(dispatcherMock.getMessages().back() == otherMessagePayload);
    }

    SECTION("When InitialLoopState receives the a message with a different payload as the starting message the state remains unchanged")
    {
        state->handle(state, std::make_unique<Message>(otherMessagePayload));
        REQUIRE(dynamic_cast<InitialLoopState*>(state.get()));
    }

    SECTION("When InitialLoopState receives the a message with a different payload as the starting message it is not relayed to the looptracker")
    {
        state->handle(state, std::make_unique<Message>(otherMessagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 0);
    }


    SECTION("InitialLoopState returns CreatedState when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(dynamic_cast<CreatedState*>(state.get()));
    }

    SECTION("InitialLoopState clears its LoopTracker when StdIn supplied")
    {
        state->handleStdin(state, std::string("any string value"));
        REQUIRE(loopTrackerMock.isCleared());
    }
}