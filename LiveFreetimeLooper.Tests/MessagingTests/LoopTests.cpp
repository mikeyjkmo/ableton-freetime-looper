#include "..\Catch\catch.h"
#include "..\..\LiveFreetimeLooper\Messaging\Message.h"

#include "..\..\LiveFreetimeLooper\Messaging\Loop.h"

#include <memory>
#include <cstdint>
using namespace LiveFreetimeLooper;

Loop createMessagelessLoopOfInterval(std::int32_t interval)
{
    Loop loop(nullptr);
    for (std::int32_t i = 0; i < interval; i++)
    {
        loop.incrementInterval();
    }

    return loop;
}

void A_Loop_of_interval_N_is_restartable_every_Mth_decrement(std::int32_t loopInterval, std::int32_t decrementInterval)
{
    auto loop = createMessagelessLoopOfInterval(loopInterval);
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

TEST_CASE("A Loop of interval 1 is restartable every 1st decrement")
{
    A_Loop_of_interval_N_is_restartable_every_Mth_decrement(1, 1);
}

TEST_CASE("A Loop of interval 2 is restartable every 2nd decrement")
{
    A_Loop_of_interval_N_is_restartable_every_Mth_decrement(2, 2);
}

TEST_CASE("A Loop of interval 5 is restartable every 5th decrement")
{
    A_Loop_of_interval_N_is_restartable_every_Mth_decrement(5, 5);
}

TEST_CASE("A Loop of interval 13 is restartable every 13th decrement")
{
    A_Loop_of_interval_N_is_restartable_every_Mth_decrement(13, 13);
}

TEST_CASE("A Loop of interval 0 is restartable every 1st decrement")
{
    // Is this the desired behaviour? //todo test broken this is checking the 0th decremeent
    A_Loop_of_interval_N_is_restartable_every_Mth_decrement(0, 1);
}

TEST_CASE("A Loop of any interval is restartable before the first decrement")
{
    for (int i = 0; i < 10; i++)
    {
        auto loop = createMessagelessLoopOfInterval(i);
        REQUIRE(loop.checkIfRestartRequired());
    }
}
