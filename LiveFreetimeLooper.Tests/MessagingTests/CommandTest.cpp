#include <unordered_set>
#include "../Catch/catch.hpp"
#include "../../LiveFreetimeLooper.Core/Messaging/Command.hpp"

using namespace LiveFreetimeLooper;


TEST_CASE("Command constructor must take a vector or size 2 or greater")
{
    std::vector<unsigned char> c1 = {'a', 'b', 'c'};
    REQUIRE_NOTHROW(Command cmd1(c1));

    std::vector<unsigned char> c2 = {'a', 'b', 'c', 'd'};
    REQUIRE_NOTHROW(Command cmd2(c2));

    std::vector<unsigned char> c3 = {};
    REQUIRE_THROWS(Command cmd3(c3));

    std::vector<unsigned char> c4 = {'a'};
    REQUIRE_THROWS(Command cmd4(c4));
}

TEST_CASE("Command operator== only takes the first two elements into consideration")
{
    Command c1('a', 'b', 'c');
    Command c2('a', 'b', 'c');
    REQUIRE(c1 == c2);

    Command c3('a', 'b', 'c');
    Command c4('a', 'b', 'd');
    REQUIRE(c3 == c4);

    Command c5('a', 'a', 'c');
    Command c6('a', 'b', 'c');
    REQUIRE(c5 != c6);

    Command c7('b', 'b', 'c');
    Command c8('a', 'b', 'c');
    REQUIRE(c7 != c8);
}

TEST_CASE("Command hash only takes the first two elements into consideration")
{
    std::unordered_set<Command> testSet;
    testSet.emplace(Command('a', 'b', 'c'));
    testSet.emplace(Command('a', 'b', 'd'));
    REQUIRE(testSet.size() == 1);

    testSet.emplace(Command('c', 'c', 'd'));
    REQUIRE(testSet.size() == 2);
}
