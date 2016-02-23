#include "../Catch/catch.hpp"

#include <memory>
#include <cstdint>

#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RecordingLoop.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/StoppedLoop.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Recording Loop yields a loop of correct interval and message")
{
    const std::int32_t interval(27);
    std::vector<unsigned char> commandContent = { 'q', 1 };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);

    for (std::int32_t i = 0; i < interval; i++)
    {
        recordingLoop.incrementInterval();
    }

    auto loop = recordingLoop.moveToRunningLoop();

    REQUIRE(loop.getInterval() == interval);
    REQUIRE(loop.getCommand() == command);
}

TEST_CASE("Recording Loop cannot move to Running Loop with a zero interval")
{
    std::vector<unsigned char> commandContent = { 'g', 1 };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);
    REQUIRE_THROWS_AS(recordingLoop.moveToRunningLoop(), std::runtime_error);
}

TEST_CASE("Recording Loop yields a stop of correct interval and message")
{
    const std::int32_t interval(65);
    std::vector<unsigned char> commandContent = { 'i', 1 };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);

    for (std::int32_t i = 0; i < interval; i++)
    {
        recordingLoop.incrementInterval();
    }

    auto loop = recordingLoop.moveToStoppedLoop();

    REQUIRE(loop.getInterval() == interval);
    REQUIRE(loop.getCommand() == command);
}

TEST_CASE("Recording Loop cannot move to Stooped Loop with a zero interval")
{
    std::vector<unsigned char> commandContent = { 'h', 1 };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);
    REQUIRE_THROWS_AS(recordingLoop.moveToStoppedLoop(), std::runtime_error);
}