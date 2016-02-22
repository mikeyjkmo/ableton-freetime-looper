#include "../Catch/catch.hpp"

#include <memory>
#include <cstdint>

#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RecordingLoop.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Recording Loop yields a loop of correct interval and message")
{
    const int interval(27);
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