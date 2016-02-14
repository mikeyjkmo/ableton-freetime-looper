#include "../Catch/catch.hpp"

#include <cstdint>
#include <memory>

#include "../Mocks/MockEventLogger.hpp"
#include "../Mocks/MockMessageDispatcher.hpp"
#include "../Mocks/MockLoopTracker.hpp"
#include "../Mocks/MockAsyncTimerFactory.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Message.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/StateBase.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/CreatedState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/InitialLoopState.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/States/RunningState.hpp"

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
    Message startingMessage(startingMessagePayload);
    std::unique_ptr<StateBase> state = std::make_unique<InitialLoopState>(
        resources, startingMessage);

    SECTION("When InitialLoopState receives the a message with the same payload as the starting message it relays the message to the dispatcher")
    {
        state->handle(state, std::make_unique<Message>(startingMessagePayload));
        REQUIRE(dispatcherMock.getMessages().size() == 1);
        REQUIRE(dispatcherMock.getMessages().back().payload == startingMessagePayload);
    }

    SECTION("When InitialLoopState receives the a message with the same payload as the starting message it is relayed to the looptracker")
    {
        state->handle(state, std::make_unique<Message>(startingMessagePayload));
        REQUIRE(loopTrackerMock.getCommandsReceived().size() == 1);
        REQUIRE(loopTrackerMock.getCommandsReceived().back()->payload == startingMessagePayload);
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
        REQUIRE(dispatcherMock.getMessages().back().payload == otherMessagePayload);
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
