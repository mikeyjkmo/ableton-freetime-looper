#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/Command.hpp"

#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.FreetimeLooper/Messaging/CommandMappings.hpp"

#include <memory>
#include <cstdint>
#include <vector>
using namespace LiveFreetimeLooper;

class LoopTrackerSteps
{
public:
    LoopTrackerSteps() : _loopTracker() {};

    void given_I_Send_A_Message(unsigned char message)
    {
        std::vector<unsigned char> command = { message };
        _loopTracker.commandReceived(Command(command));
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
        auto restartCommands = _loopTracker.getNextRestartCommands();
        REQUIRE(MessageCount(message, restartCommands) == 1);
    }

    void then_The_Message_Is_Restartable_On_Every_Nth_Interval(unsigned char message, int number)
    {
        for (std::int32_t i = 0;i < number * 10 + 1;i++)
        {
            auto restartCommands = _loopTracker.getNextRestartCommands();
            if (i%number == 0)
            {
                CAPTURE(i);
                REQUIRE(MessageCount(message, restartCommands) == 1);
            }
            else
            {
                CAPTURE(i);
                REQUIRE(MessageCount(message, restartCommands) == 0);
            }

            _loopTracker.incrementInterval();
        }
    }

    void then_The_Message_Is_Not_Restartable_For_N_Intervals(unsigned char message, int number)
    {
        for (std::int32_t i = 0;i < number * 10 + 1;i++)
        {
            auto restartCommands = _loopTracker.getNextRestartCommands();
            CAPTURE(i);
            REQUIRE(MessageCount(message, restartCommands) == 0);
        }
    }

private :

    LoopTracker _loopTracker;

    std::int32_t MessageCount(unsigned char message, const std::vector<Command>& restartCommands)
    {
        std::int32_t matchingItems = 0;
        for (const auto command : restartCommands)
        {
            if (command.content.size() == 1 && *command.content.begin() == message) ++matchingItems;
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
    std::vector<unsigned char> Command;
    std::int32_t StartInterval;
    LoopInfo(std::int32_t interval, std::int32_t startInterval, std::vector<unsigned char> command) :
        Interval(interval), Command(command), StartInterval(startInterval) {}
};

bool ContainsCommand(std::vector<Command> commands, std::vector<unsigned char>& expectedCommand)
{
    for (auto &command : commands)
    {
        if (command.content == expectedCommand)
        {
            return true;
        }
    }

    return false;
}

TEST_CASE("Loop Tracker can track multiple loops concurrently")
{
    std::vector<unsigned char> loopACommand = { 'A' };
    std::vector<unsigned char> loopBCommand = { 'B' };
    std::vector<unsigned char> loopCCommand = { 'C' };
    std::vector<unsigned char> loopDCommand = { 'D' };
    std::vector<unsigned char> loopECommand = { 'E' };
    std::vector<unsigned char> loopFCommand = { 'F' };
    std::vector<unsigned char> loopGCommand = { 'G' };


    std::vector<LoopInfo> testLoops =
    {
        LoopInfo(2, 0, loopACommand),
        LoopInfo(2, 3, loopBCommand),
        LoopInfo(7, 12, loopCCommand),
        LoopInfo(2, 1, loopDCommand),
        LoopInfo(2, 1, loopECommand),
        LoopInfo(1, 9, loopFCommand)
    };

    LoopTracker tracker;

    for (std::int32_t i = 0;i < 100;i++)
    {
        std::vector<std::vector<unsigned char>> commandsToSend;
        std::vector<std::vector<unsigned char>> expectedRestartCommands;
        for (auto &loop : testLoops)
        {
            // Is time to start or stop recording
            if (i == loop.StartInterval || i == loop.StartInterval + loop.Interval)
            {
                commandsToSend.push_back(loop.Command);
            }

            // Not finished recording
            if (i < loop.StartInterval + loop.Interval) continue;

            // Is it the correct interval to get a restart message
            if ((i - loop.StartInterval) % loop.Interval == 0)
            {
                expectedRestartCommands.push_back(loop.Command);
            }
        }

        for (auto &command : commandsToSend)
        {
            tracker.commandReceived(Command(command));
        }

        auto restartCommands = tracker.getNextRestartCommands();

        CAPTURE(i);
        REQUIRE(restartCommands.size() == expectedRestartCommands.size());

        for (auto &expectedCommand : expectedRestartCommands)
        {
            CAPTURE(i);
            REQUIRE(ContainsCommand(restartCommands, expectedCommand));
        }

        tracker.incrementInterval();
    }
}
