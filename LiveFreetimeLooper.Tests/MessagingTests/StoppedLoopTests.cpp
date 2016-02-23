#include "../Catch/catch.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/StoppedLoop.hpp"

using namespace LiveFreetimeLooper;

TEST_CASE("Stopped Loop yields a stopped loop of correct interval and message")
{
    const std::int32_t interval(5);
    Command command('y', 1, 0);
    StoppedLoop loop(command, interval);

    auto runningLoop = loop.moveToRunningLoop();

    REQUIRE(loop.getInterval() == interval);
    REQUIRE(loop.getCommand() == command);
}