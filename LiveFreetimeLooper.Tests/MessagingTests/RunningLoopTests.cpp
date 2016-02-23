#include "../Catch/catch.hpp"

#include <memory>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/StoppedLoop.hpp"

using namespace LiveFreetimeLooper;

void a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(std::int32_t loopInterval, std::int32_t decrementInterval)
{
    auto loop = RunningLoop(Command('a', 1, 0), loopInterval);
    for (std::int32_t j = 0; j < 5; j++)
    {
        bool isRestartableBeforeNDecrements = false;
        for (std::int32_t i = 0; i < decrementInterval - 1; i++)
        {
            loop.decrementNextRestartWait();
            isRestartableBeforeNDecrements |= loop.checkIfRestartRequired();
        }

        loop.decrementNextRestartWait();
        bool isRestartableOnNthDecrements = loop.checkIfRestartRequired();

        REQUIRE_FALSE(isRestartableBeforeNDecrements);
        REQUIRE(isRestartableOnNthDecrements);
    }
}

TEST_CASE("A RunningLoop of interval 1 is restartable every 1st decrement")
{
    a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(1, 1);
}

TEST_CASE("A RunningLoop of interval 2 is restartable every 2nd decrement")
{
    a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(2, 2);
}

TEST_CASE("A RunningLoop of interval 5 is restartable every 5th decrement")
{
    a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(5, 5);
}

TEST_CASE("A RunningLoop of interval 13 is restartable every 13th decrement")
{
    a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(13, 13);
}

TEST_CASE("A RunningLoop of interval less than 1 cannot be created")
{
    REQUIRE_THROWS_AS(RunningLoop(Command('a', 1, 0), 0),std::runtime_error);
    REQUIRE_THROWS_AS(RunningLoop(Command('a', 1, 0), -1), std::runtime_error);
    REQUIRE_THROWS_AS(RunningLoop(Command('a', 1, 0), -54), std::runtime_error);
}

TEST_CASE("A RunningLoop of any interval is restartable before the first decrement")
{
    for (int i = 1; i < 10; i++)
    {
        auto loop = RunningLoop(Command('a', 1, 0), i);
        REQUIRE(loop.checkIfRestartRequired());
    }
}

TEST_CASE("Running Loop yields a stopped loop of correct interval and message")
{
    const std::int32_t interval(5);
    Command command('y', 1, 0);
    RunningLoop loop(command, interval);

    auto stoppedLoop = loop.moveToStoppedLoop();

    REQUIRE(loop.getInterval() == interval);
    REQUIRE(loop.getCommand() == command);
}