// Hack to make available the code under test
// todo do this properly http://stackoverflow.com/questions/9963894/visual-studio-include-h-or-cpp-in-two-project-solution
// https://msdn.microsoft.com/en-us/library/hh419385.aspx

#include "Logging\MessageDispatchedEvent.cpp"
#include "Logging\StateChangedEvent.cpp"
#include "Messaging\Loop.cpp"
#include "Messaging\LoopTracker.cpp"
#include "Messaging\MessageReceiver.cpp"
#include "States\CreatedState.cpp"
#include "States\InitialLoopState.cpp"
#include "States\InitialLoopWaitingState.cpp"
#include "States\RunningState.cpp"
#include "Utilities\AsyncTimer.cpp"
#include "Utilities\Stopwatch.cpp"