#include "Session/Session.h"

using namespace LiveFreetimeLooper;

int main(int argc, char *argv[])
{
    Session session(argc, argv);
    return session.run();
}