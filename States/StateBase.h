#pragma once

#include <string>
#include <memory>

namespace AbletonProject
{

    class StateBase
    {
    protected:
        StateBase();
    public:
        virtual ~StateBase() = default;
        virtual void handle(std::unique_ptr<StateBase>& state, std::string message) = 0;
    };

}
