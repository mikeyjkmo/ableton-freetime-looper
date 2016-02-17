#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/CommandMappings.hpp"

using namespace LiveFreetimeLooper;


TEST_CASE("CommandsMappings::getMessageType() returns the correct message type.")
{
    std::unordered_map<Command, Command> startsToStops = {
        {Command(1, 2, 3), Command(4, 5, 6)},
        {Command(7, 8, 9), Command(10, 11, 12)}
    };
    CommandMappings cm(startsToStops);

    REQUIRE(cm.getMessageType(Command(1, 2, 3)) == MessageType::START);
    REQUIRE(cm.getMessageType(Command(4, 5, 6)) == MessageType::STOP);
    REQUIRE(cm.getMessageType(Command(7, 8, 9)) == MessageType::START);
    REQUIRE(cm.getMessageType(Command(10, 11, 12)) == MessageType::STOP);
}

TEST_CASE("CommandsMappings::getStartCommand() returns the correct message or throws an exception.")
{
    std::unordered_map<Command, Command> startsToStops = {
        {Command(1, 2, 3), Command(4, 5, 6)},
        {Command(7, 8, 9), Command(10, 11, 12)}
    };
    CommandMappings cm(startsToStops);

    REQUIRE(cm.getStartCommand(Command(4, 5, 6)) == Command(1, 2, 3));
    REQUIRE(cm.getStartCommand(Command(10, 11, 12)) == Command(7, 8, 9));
    REQUIRE_THROWS(cm.getStartCommand(Command(0, 0, 0)));
    REQUIRE_THROWS(cm.getStartCommand(Command(1, 2, 3)));
    REQUIRE_THROWS(cm.getStartCommand(Command(7, 8, 9)));
}
