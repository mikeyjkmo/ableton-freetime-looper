#pragma once

#include "Utilities\IAsyncTimer.h"

class MockAsyncTimer final : public LiveFreetimeLooper::IAsyncTimer
{
private:

public:
    MockAsyncTimer();

    void start();
    void stop();
};