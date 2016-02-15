#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Command.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/RunningLoop.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/RecordingLoop.hpp"

#include <memory>
#include <cstdint>
using namespace LiveFreetimeLooper;

TEST_CASE("Recording Loop yields a loop of correct interval and message")
{
    std::vector<unsigned char> commandContent = { 'q' };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);

    for (std::int32_t i = 0; i < 27; i++)
    {
        recordingLoop.incrementInterval();
    }

    auto loop = recordingLoop.moveToRunningLoop();

    REQUIRE(loop.getInterval() == 27);
    REQUIRE(loop.getCommand() == command);
}

TEST_CASE("Recording Loop cannot move to Running Loop with a zero interval")
{
    std::vector<unsigned char> commandContent = { 'g' };
    Command command(commandContent);
    RecordingLoop recordingLoop(command);
    REQUIRE_THROWS_AS(recordingLoop.moveToRunningLoop(), std::runtime_error);
}
