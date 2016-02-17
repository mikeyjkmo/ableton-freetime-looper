#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/RunningLoop.hpp"

#include <memory>
#include <cstdint>
#include <stdexcept>
#include <vector>
using namespace LiveFreetimeLooper;

void a_RunningLoop_of_interval_N_is_restartable_every_Mth_decrement(std::int32_t loopInterval, std::int32_t decrementInterval)
{
    auto loop = RunningLoop(Command(std::vector<unsigned char>()), loopInterval);
    for (std::int32_t j = 0;j < 5;j++)
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
    REQUIRE_THROWS_AS(RunningLoop(Command(std::vector<unsigned char>()), 0),std::runtime_error);
    REQUIRE_THROWS_AS(RunningLoop(Command(std::vector<unsigned char>()), -1), std::runtime_error);
    REQUIRE_THROWS_AS(RunningLoop(Command(std::vector<unsigned char>()), -54), std::runtime_error);
}

TEST_CASE("A RunningLoop of any interval is restartable before the first decrement")
{
    for (int i = 1; i < 10; i++)
    {
        auto loop = RunningLoop(Command(std::vector<unsigned char>()), i);
        REQUIRE(loop.checkIfRestartRequired());
    }
}
