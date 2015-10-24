#include "MessageDispatcher.h"

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher()
    {
    }

    void MessageDispatcher::sendMessage(std::string message)
    {
        std::cout << message << std::endl;
    }

}
