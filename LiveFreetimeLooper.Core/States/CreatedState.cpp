#include "CreatedState.hpp"
#include "InitialLoopWaitingState.hpp"
#include "../Logging/IEventLogger.hpp"
#include "../Logging/StateChangedEvent.hpp"

namespace LiveFreetimeLooper
{
    CreatedState::CreatedState(StateResources& resources) : _resources(resources)
    {
    }

    void CreatedState::handle(std::unique_ptr<StateBase>&, std::unique_ptr<StartMessage> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
    }

    void CreatedState::handle(std::unique_ptr<StateBase>&, std::unique_ptr<StopMessage> message)
    {
        _resources.messageDispatcher.sendMidiMessage(message.get());
    }

    void CreatedState::handleStdin(std::unique_ptr<StateBase>& state, const std::string&)
    {
        _resources.logger.log(std::make_unique<StateChangedEvent>(
            std::string("StdIn. Progress: Created -> InitialLoopWaiting"),
            std::string("CreatedState")));
        state = std::make_unique<InitialLoopWaitingState>(_resources);
    }

}
