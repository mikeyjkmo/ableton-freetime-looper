#include "Tests\Catch\catch.h"
#include "Messaging\Message.h"

#include "Messaging\LoopTracker.cpp"

#include <memory>
#include <cstdint>
#include <vector>
using namespace LiveFreetimeLooper;

void When_a_message_is_sent_on_the_1st_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(std::int32_t interval)
{
    std::vector<unsigned char> messagePayload1 = { 0, 0 };
    LoopTracker tracker;

    tracker.commandReceived(std::make_unique<Message>(messagePayload1));
    for (std::int32_t i = 0;i < interval;i++)
    {
        tracker.incrementInterval();
    }
    tracker.commandReceived(std::make_unique<Message>(messagePayload1));

    for (std::int32_t i = 0;i < interval * 10 + 1;i++)
    {
        auto restartMessages = tracker.getNextRestartMessages();
        if (i%interval == 0)
        {
            CAPTURE(i);
            REQUIRE(restartMessages.size() == 1);
        }
        else
        {
            CAPTURE(i);
            REQUIRE(restartMessages.size() == 0);
        }
        tracker.incrementInterval();
    }
}

TEST_CASE("When a message is sent on the 1st and Nth interval, it then appears as restartable every Nth interval")
{
    SECTION("N=1")
    {
        When_a_message_is_sent_on_the_1st_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(1);
    }
    SECTION("N=5")
    {
        When_a_message_is_sent_on_the_1st_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(5);
    }
}

TEST_CASE("When a new message is sent twice in the same interval, err todo, what happens?")
{
    // todo Apparently, it acts the same as a loop of interval 1

    std::vector<unsigned char> messagePayload1 = { 0, 0 };
    LoopTracker tracker;

    tracker.commandReceived(std::make_unique<Message>(messagePayload1));
    tracker.commandReceived(std::make_unique<Message>(messagePayload1));

    tracker.incrementInterval();
    for (std::int32_t i = 0;i < 5;i++)
    {
        auto restartMessages = tracker.getNextRestartMessages();
        CAPTURE(i);
        REQUIRE(restartMessages.size() == 1);
    }
}

struct LoopInfo
{
public:
    std::int32_t Interval;
    std::vector<unsigned char> MessagePayload;
    std::int32_t StartInterval;
    LoopInfo(std::int32_t interval, std::int32_t startInterval, std::vector<unsigned char> messagePayload) :
        Interval(interval), StartInterval(startInterval), MessagePayload(messagePayload) {}
};

bool ContainsMessagePayload(std::vector<Message*> messages, std::vector<unsigned char> expectedMessagePayload)
{
    for (auto &message : messages)
    {
        if ((*message).payload == expectedMessagePayload)
        {
            return true;
        }
    }

    return false;
}

TEST_CASE("Loop Tracker can track multiple loops")
{
    std::vector<unsigned char> messagePayload1 = { 0, 0 };
    std::vector<unsigned char> messagePayload2 = { 0, 1 };
    std::vector<unsigned char> messagePayload3 = { 1, 1 };
    std::vector<unsigned char> messagePayload4 = { 1, 2 };
    std::vector<unsigned char> messagePayload5 = { 2, 2 };
    std::vector<unsigned char> messagePayload6 = { 3, 2 };


    std::vector<LoopInfo> testLoops =
        {
            LoopInfo(2, 0, messagePayload1),
            LoopInfo(2, 3, messagePayload2),
            LoopInfo(7, 12, messagePayload3),
            LoopInfo(2, 1, messagePayload4),
            LoopInfo(2, 1, messagePayload5),
            LoopInfo(1, 9, messagePayload6)
        };

    LoopTracker tracker;

    for (std::int32_t i = 0;i < 100;i++)
    {
        std::vector<std::vector<unsigned char>> messagePayloadsToSend;
        std::vector<std::vector<unsigned char>> expectedRestartMessages;
        for (auto &loop : testLoops)
        {
            // Is time to start or stop recording
            if (i == loop.StartInterval || i == loop.StartInterval + loop.Interval)
            {
                messagePayloadsToSend.push_back(loop.MessagePayload);
            }

            // Not finished recording
            if (i < loop.StartInterval + loop.Interval) continue;

            // Is it the correct interval to get a restart message
            if ((i - loop.StartInterval) % loop.Interval == 0)
            {
                expectedRestartMessages.push_back(loop.MessagePayload);
            }
        }

        for (auto &message : messagePayloadsToSend)
        {
            tracker.commandReceived(std::make_unique<Message>(message));
        }

        auto restartMessages = tracker.getNextRestartMessages();

        CAPTURE(i);
        REQUIRE(restartMessages.size() == expectedRestartMessages.size());

        for (auto &expectedMessage : expectedRestartMessages)
        {
            CAPTURE(i);
            REQUIRE(ContainsMessagePayload(restartMessages, expectedMessage));
        }

        tracker.incrementInterval();
    }
}