// AbletonProject.cpp : Defines the entry point for the console application.
//
#include "Session/Session.h"

using namespace AbletonProject;

int main(int argc, char *argv[])
{
    Session session(argc, argv);
    return session.run();
}
