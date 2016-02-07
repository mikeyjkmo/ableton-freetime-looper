// Hack to make available the code under test
// todo do this properly http://stackoverflow.com/questions/9963894/visual-studio-include-h-or-cpp-in-two-project-solution
// https://msdn.microsoft.com/en-us/library/hh419385.aspx

#include "..\LiveFreetimeLooper\Logging\MessageDispatchedEvent.cpp"
#include "..\LiveFreetimeLooper\Logging\StateChangedEvent.cpp"
#include "..\LiveFreetimeLooper\Messaging\Loop.cpp"
#include "..\LiveFreetimeLooper\Messaging\LoopTracker.cpp"
#include "..\LiveFreetimeLooper\Messaging\MessageReceiver.cpp"
#include "..\LiveFreetimeLooper\States\CreatedState.cpp"
#include "..\LiveFreetimeLooper\States\InitialLoopState.cpp"
#include "..\LiveFreetimeLooper\States\InitialLoopWaitingState.cpp"
#include "..\LiveFreetimeLooper\States\RunningState.cpp"
#include "..\LiveFreetimeLooper\Utilities\AsyncTimer.cpp"
#include "..\LiveFreetimeLooper\Utilities\AsyncTimerFactory.cpp"
#include "..\LiveFreetimeLooper\Utilities\Stopwatch.cpp"