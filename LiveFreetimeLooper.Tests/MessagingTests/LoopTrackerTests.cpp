#include "..\Catch\catch.hpp"
#include "..\..\LiveFreetimeLooper\Messaging\Message.hpp"

#include "..\..\LiveFreetimeLooper\Messaging\LoopTracker.hpp"

#include <memory>
#include <cstdint>
#include <vector>
using namespace LiveFreetimeLooper;

void When_a_message_is_sent_on_the_0th_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(std::int32_t interval)
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

TEST_CASE("When a message is sent on the 0th and 2th interval, it then appears as restartable on the 2nd, 4th, 6th etc interval")
{
    // todo would be nice to split this up into send, wait, send, assert restartablity
    When_a_message_is_sent_on_the_0th_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(2);
}

TEST_CASE("When a message is sent on the 0st and 5th interval, it then appears as restartable on the 5nd, 10th, 15th etc interval")
{
    When_a_message_is_sent_on_the_0th_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(5);
}

TEST_CASE("When a message is sent on the 0st and 1th interval, it then appears as restartable on every interval")
{
    When_a_message_is_sent_on_the_0th_and_Nth_interval_it_then_appears_as_restartable_every_Nth_interval(1);
}

TEST_CASE("When a message is sent on the 0st and 0th interval, it then appears as restartable on every interval")
{
    // Is this the desired behaviour?

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

TEST_CASE("The message is interval is decided by the first two messages, all further messages are ignored")
{
    // todo
}

TEST_CASE("The message is restartable immediately after the second message is received")
{
    std::vector<unsigned char> messagePayload1 = { 0, 0 };
    LoopTracker tracker;

    tracker.commandReceived(std::make_unique<Message>(messagePayload1));
    tracker.incrementInterval();
    tracker.incrementInterval();
    tracker.commandReceived(std::make_unique<Message>(messagePayload1));
    REQUIRE(tracker.getNextRestartMessages().size() == 1);
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

TEST_CASE("Loop Tracker can track multiple loops concurrently")
{
    std::vector<unsigned char> loopAPayload = { 'A' };
    std::vector<unsigned char> loopBPayload = { 'B' };
    std::vector<unsigned char> loopCPayload = { 'C' };
    std::vector<unsigned char> loopDPayload = { 'D' };
    std::vector<unsigned char> loopEPayload = { 'E' };
    std::vector<unsigned char> loopFPayload = { 'F' };
    std::vector<unsigned char> loopGPayload = { 'G' };


    std::vector<LoopInfo> testLoops =
    {
        LoopInfo(2, 0, loopAPayload),
        LoopInfo(2, 3, loopBPayload),
        LoopInfo(7, 12, loopCPayload),
        LoopInfo(2, 1, loopDPayload),
        LoopInfo(2, 1, loopEPayload),
        LoopInfo(1, 9, loopFPayload)
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