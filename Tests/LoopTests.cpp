#include "Tests\Catch\catch.h"
#include "Messaging\Message.h"

#include "Messaging\Loop.cpp"

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

void A_Loop_of_interval_N_is_restartable_every_Nth_decrement(std::int32_t loopInterval)
{
    auto loop = createMessagelessLoopOfInterval(loopInterval);
    for (std::int32_t j = 0;j < 5;j++)
    {
        bool isRestartableBeforeNDecrements = false;
        for (std::int32_t i = 0; i < loopInterval - 1; i++)
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

TEST_CASE("A Loop of interval N is restartable every Nth decrement", "[Messaging]") {
    SECTION("N=1")
    {
        A_Loop_of_interval_N_is_restartable_every_Nth_decrement(1);
    }
    SECTION("N=2")
    {
        A_Loop_of_interval_N_is_restartable_every_Nth_decrement(2);
    }
    SECTION("N=13")
    {
        A_Loop_of_interval_N_is_restartable_every_Nth_decrement(13);
    }
    SECTION("N=10")
    {
        A_Loop_of_interval_N_is_restartable_every_Nth_decrement(10);
    }
}

// todo, what is the meaning of a loop of interval 0 (rather than interval 1). They both seem to act the same
