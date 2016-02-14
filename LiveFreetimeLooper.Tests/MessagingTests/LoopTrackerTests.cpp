#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Message.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/CommandMappings.hpp"

#include <memory>
#include <cstdint>
#include <vector>
using namespace LiveFreetimeLooper;

class LoopTrackerSteps
{
public:
    LoopTrackerSteps() : _loopTracker(CommandMappings()) {};

    void given_I_Send_A_Message(unsigned char message)
    {
        std::vector<unsigned char> messagePayload1 = { message };
        _loopTracker.commandReceived(std::make_unique<Message>(messagePayload1));
    }

    void given_I_Wait_N_Intervals(unsigned char number)
    {
        for (std::int32_t i = 0;i < number;i++)
        {
            _loopTracker.incrementInterval();
        }
    }

    void given_I_Wait_One_Interval()
    {
        given_I_Wait_N_Intervals(1);
    }

    void then_The_Message_Is_Restartable(unsigned char message)
    {
        auto restartMessages = _loopTracker.getNextRestartMessages();
        REQUIRE(MessageCount(message, restartMessages) == 1);
    }

    void then_The_Message_Is_Restartable_On_Every_Nth_Interval(unsigned char message, int number)
    {
        for (std::int32_t i = 0;i < number * 10 + 1;i++)
        {
            auto restartMessages = _loopTracker.getNextRestartMessages();
            if (i%number == 0)
            {
                CAPTURE(i);
                REQUIRE(MessageCount(message, restartMessages) == 1);
            }
            else
            {
                CAPTURE(i);
                REQUIRE(MessageCount(message, restartMessages) == 0);
            }

            _loopTracker.incrementInterval();
        }
    }

    void then_The_Message_Is_Not_Restartable_For_N_Intervals(unsigned char message, int number)
    {
        for (std::int32_t i = 0;i < number * 10 + 1;i++)
        {
            auto restartMessages = _loopTracker.getNextRestartMessages();
            CAPTURE(i);
            REQUIRE(MessageCount(message, restartMessages) == 0);
        }
    }

private :

    LoopTracker _loopTracker;

    std::int32_t MessageCount(unsigned char message, const std::vector<LiveFreetimeLooper::Message*>& restartMessages)
    {
        std::int32_t matchingItems = 0;
        for (const auto item : restartMessages)
        {
            if (item->payload.size() == 1 && *item->payload.begin() == message) ++matchingItems;
        }

        return matchingItems;
    }
};


TEST_CASE("When a message is sent on the 0th and 2th interval, it then appears as restartable on the 2nd, 4th, 6th etc interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Message('a');
    test.given_I_Wait_One_Interval();
    test.given_I_Wait_One_Interval();

    test.given_I_Send_A_Message('a');

    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('a', 2);
}

TEST_CASE("When a message is sent on the 0st and 5th interval, it then appears as restartable on the 5nd, 10th, 15th etc interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Message('b');
    test.given_I_Wait_N_Intervals(5);
    test.given_I_Send_A_Message('b');

    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('b', 5);
}

TEST_CASE("When a message is sent on the 0st and 1th interval, it then appears as restartable on every interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Message('c');
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Message('c');

    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('c', 1);

}


TEST_CASE("When a message is sent on the same interval, that is ignored and  the next message starts a new recording")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Message('d');
    test.given_I_Send_A_Message('d');
    test.then_The_Message_Is_Not_Restartable_For_N_Intervals('d', 10);
    test.given_I_Send_A_Message('d');
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Message('d');

    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('d', 1);
}

TEST_CASE("The interval is decided by the first two identical messages")
{
    LoopTrackerSteps test;
    test.given_I_Send_A_Message('g');
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Message('e');
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Message('f');
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Message('e');
    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('e', 6);
}

TEST_CASE("On receipt of a message that matches a running loop, the message is relayed but ignored")
{
    LoopTrackerSteps test;
    test.given_I_Send_A_Message('y');
    test.given_I_Wait_N_Intervals(4);
    test.given_I_Send_A_Message('y');
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Message('y');
    test.given_I_Wait_N_Intervals(2);
    test.given_I_Send_A_Message('y');
    test.then_The_Message_Is_Restartable_On_Every_Nth_Interval('y', 4);
}

TEST_CASE("The message is restartable immediately after the second message is received")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Message('h');
    test.given_I_Wait_N_Intervals(2);
    test.given_I_Send_A_Message('h');
    test.then_The_Message_Is_Restartable('h');
}

struct LoopInfo
{
public:
    std::int32_t Interval;
    std::vector<unsigned char> MessagePayload;
    std::int32_t StartInterval;
    LoopInfo(std::int32_t interval, std::int32_t startInterval, std::vector<unsigned char> messagePayload) :
        Interval(interval), MessagePayload(messagePayload), StartInterval(startInterval) {}
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

    CommandMappings commandMappings;
    LoopTracker tracker(commandMappings);

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
