#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"

#include "../../LiveFreetimeLooper.Core/Messaging/LoopTracker.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/CommandMappings.hpp"

#include <memory>
#include <cstdint>
#include <vector>
using namespace LiveFreetimeLooper;

class LoopTrackerSteps
{
public:
    LoopTrackerSteps() : _loopTracker() {};

    void given_I_Send_A_Start_Command(const std::vector<unsigned char>& command)
    {
        _loopTracker.startCommand(Command(command));
    }

    void given_I_Send_A_Stop_Command(
        const std::vector<unsigned char>& correspondingStartCommand
        )
    {
        when_I_Send_A_Stop_Command(correspondingStartCommand);
    }

    void when_I_Send_A_Stop_Command(
        const std::vector<unsigned char>& correspondingStartCommand
        )
    {
        _loopTracker.stopCommand(Command(correspondingStartCommand));
    }

    void given_I_Wait_N_Intervals(unsigned int number)
    {
        for (unsigned int i = 0; i < number; ++i)
        {
            _loopTracker.incrementInterval();
        }
    }

    void given_I_Wait_One_Interval()
    {
        given_I_Wait_N_Intervals(1);
    }

    void then_The_Command_Is_Restartable(const std::vector<unsigned char>& message)
    {
        auto restartCommands = _loopTracker.getNextRestartCommands();
        REQUIRE(MessageCount(message, restartCommands) == 1);
    }

    void then_The_Command_Is_Restartable_On_Every_Nth_Interval(
        const std::vector<unsigned char>& message, int number)
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

    void then_The_Command_Is_Not_Restartable_For_N_Intervals(
        const std::vector<unsigned char>& message, int number)
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

    std::int32_t MessageCount(
        const std::vector<unsigned char>& message, const std::vector<Command>& restartCommands)
    {
        std::int32_t matchingItems = 0;
        for (const auto command : restartCommands)
        {
            if (command.content == message) ++matchingItems;
        }

        return matchingItems;
    }
};


TEST_CASE("When a start command is sent on the 0th and 2th interval,"
    " the loop is restartable on the 2nd, 4th, 6th etc interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Start_Command({'a', 1});
    test.given_I_Wait_One_Interval();
    test.given_I_Wait_One_Interval();

    test.given_I_Send_A_Start_Command({'a', 1});

    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'a', 1}, 2);
}

TEST_CASE("When a start command is sent on the 0st and 5th interval, "
    " the loop is restartable on the 5nd, 10th, 15th etc interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Start_Command({'b', 1});
    test.given_I_Wait_N_Intervals(5);
    test.given_I_Send_A_Start_Command({'b', 1});

    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'b', 1}, 5);
}

TEST_CASE("When a start command is sent on the 0st and 1th interval, "
    " the loop is restartable on every interval")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Start_Command({'c', 1});
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Start_Command({'c', 1});

    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'c', 1}, 1);

}


TEST_CASE("When two start commands are send on the same interval"
    "both are ignored")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Start_Command({'d', 1});
    test.given_I_Send_A_Start_Command({'d', 1});
    test.then_The_Command_Is_Not_Restartable_For_N_Intervals({'d', 1}, 10);

    // The start command can be sent in again later, with the usual effect.
    test.given_I_Send_A_Start_Command({'d', 1});
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Start_Command({'d', 1});

    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'d', 1}, 1);
}

TEST_CASE("The loop is defined by two matching commands")
{
    LoopTrackerSteps test;
    test.given_I_Send_A_Start_Command({'g', 1});
    test.given_I_Wait_One_Interval();
    test.given_I_Send_A_Start_Command({'e', 1});
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Start_Command({'f', 1});
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Start_Command({'e', 1});
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'e', 1}, 6);
}

TEST_CASE("A start command that matches a running loop is ignored")
{
    LoopTrackerSteps test;
    test.given_I_Send_A_Start_Command({'y', 1});
    test.given_I_Wait_N_Intervals(4);
    test.given_I_Send_A_Start_Command({'y', 1});
    test.given_I_Wait_N_Intervals(3);
    test.given_I_Send_A_Start_Command({'y', 1});
    test.given_I_Wait_N_Intervals(2);
    test.given_I_Send_A_Start_Command({'y', 1});
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval({'y', 1}, 4);
}

TEST_CASE("The loop is restartable immediately after the second start command is received")
{
    LoopTrackerSteps test;

    test.given_I_Send_A_Start_Command({'h', 1});
    test.given_I_Wait_N_Intervals(2);
    test.given_I_Send_A_Start_Command({'h', 1});
    test.then_The_Command_Is_Restartable({'h', 1});
}

TEST_CASE("When a running loop is stopped, it should stop being restartable")
{
    LoopTrackerSteps test;
    std::vector<unsigned char> command = { 'i', 1 };
    const std::int32_t interval = 2;

    test.given_I_Send_A_Start_Command(command);
    test.given_I_Wait_N_Intervals(interval);
    test.given_I_Send_A_Start_Command(command);
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);

    test.when_I_Send_A_Stop_Command(command);

    test.then_The_Command_Is_Not_Restartable_For_N_Intervals(command, 10);

    SECTION("and the loop can be started again")
    {
        test.given_I_Send_A_Start_Command(command);
        test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);
    }
}

TEST_CASE("When a recording loop is stopped, "
    "it should stop being restartable")
{
    LoopTrackerSteps test;
    std::vector<unsigned char> command = { 'j', 1 };
    const std::int32_t interval = 6;

    test.given_I_Send_A_Start_Command(command);
    test.given_I_Wait_N_Intervals(interval);

    test.given_I_Send_A_Stop_Command(command);
    
    test.then_The_Command_Is_Not_Restartable_For_N_Intervals(command, 20);

    SECTION("and the loop can be started again")
    {
        test.given_I_Send_A_Start_Command(command);
        test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);
    }
}

TEST_CASE("When a recording loop is stopped on the same interval as it is started "
    "it is forgotten about")
{
    LoopTrackerSteps test;
    std::vector<unsigned char> command = { 'j', 1 };

    test.given_I_Send_A_Start_Command(command);
    test.given_I_Send_A_Stop_Command(command);

    test.then_The_Command_Is_Not_Restartable_For_N_Intervals(command, 10);

    // Clean Slate
    const std::int32_t interval = 4;
    test.given_I_Send_A_Start_Command(command);
    test.given_I_Wait_N_Intervals(interval);
    test.given_I_Send_A_Start_Command(command);
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);
}

TEST_CASE("When a stopped loop is stopped, "
    "nothing happens and its later restart is unaffected")
{
    LoopTrackerSteps test;
    std::vector<unsigned char> command = { 'l', 1 };
    const std::int32_t interval = 3;

    test.given_I_Send_A_Start_Command(command);
    test.given_I_Wait_N_Intervals(interval);
    test.given_I_Send_A_Start_Command(command);
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);
    test.when_I_Send_A_Stop_Command(command);
    test.then_The_Command_Is_Not_Restartable_For_N_Intervals(command, 10);

    test.when_I_Send_A_Stop_Command(command);
    
    // the loop is stopped
    test.then_The_Command_Is_Not_Restartable_For_N_Intervals(command, 10);

    // The loop can still be restarted
    test.given_I_Send_A_Start_Command(command);
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, interval);
}

TEST_CASE("When an unknown loop is stopped, "
    "nothing happens and the future definition is unaffected")
{
    LoopTrackerSteps test;
    std::vector<unsigned char> command = { 'k', 1 };

    test.given_I_Send_A_Stop_Command(command);
    test.given_I_Wait_One_Interval();

    //Clean slate
    test.given_I_Send_A_Start_Command(command);
    test.given_I_Wait_N_Intervals(2);
    test.given_I_Send_A_Start_Command(command);
    test.then_The_Command_Is_Restartable_On_Every_Nth_Interval(command, 2);
}

TEST_CASE("Loop tracker does not depend on start commands being in scope")
{
    LoopTracker _loopTracker;

    { // inner scope
        std::vector<unsigned char> content = { 'k', 1 };
        Command command(content);

        _loopTracker.startCommand(command);
        _loopTracker.incrementInterval();
        _loopTracker.startCommand(command);

        content[0] = 'l';
        command.content[0] = 'j';
    }

    auto restartCommands = _loopTracker.getNextRestartCommands();
    REQUIRE(restartCommands.size() == 1);
    REQUIRE(restartCommands.back().content[0] == 'k');
}

struct LoopInfo
{
public:
    std::int32_t Interval;
    std::vector<unsigned char> Command;
    std::int32_t StartInterval;

    LoopInfo(
        std::int32_t interval,
        std::int32_t startInterval,
        std::vector<unsigned char> command
        ) :
        Interval(interval),
        Command(command),
        StartInterval(startInterval)
    {
    }
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
    std::vector<unsigned char> loopACommand = {'A', 1};
    std::vector<unsigned char> loopBCommand = {'B', 1};
    std::vector<unsigned char> loopCCommand = {'C', 1};
    std::vector<unsigned char> loopDCommand = {'D', 1};
    std::vector<unsigned char> loopECommand = {'E', 1};
    std::vector<unsigned char> loopFCommand = {'F', 1};
    std::vector<unsigned char> loopGCommand = {'G', 1};


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

    for (std::int32_t i = 0; i < 100; ++i)
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
            tracker.startCommand(Command(command));
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