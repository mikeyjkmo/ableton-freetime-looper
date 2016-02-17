#include "../LiveFreetimeLooper.Core/Session/Session.hpp"

using namespace LiveFreetimeLooper;

int main(int argc, char *argv[])
{
    Session session(argc, argv);
    return session.run();
}
