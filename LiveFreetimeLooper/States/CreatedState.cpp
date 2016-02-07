#include "CreatedState.h"
#include "InitialLoopWaitingState.h"
#include "../Logging/IEventLogger.h"
#include "../Logging/StateChangedEvent.h"

namespace LiveFreetimeLooper
{
    CreatedState::CreatedState(StateResources& resources) : _resources(resources)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>& state, std::unique_ptr<Message> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
    }

    void CreatedState::handleStdin(std::unique_ptr<StateBase>& state, std::string& input)
    {
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn. Progress: Created -> InitialLoopWaiting"),
            std::string("CreatedState")));
        state = std::make_unique<InitialLoopWaitingState>(_resources);
    }

}
