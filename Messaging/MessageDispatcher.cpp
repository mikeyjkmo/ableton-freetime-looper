#include "MessageDispatcher.h"

#include <iostream>

namespace AbletonProject
{

    MessageDispatcher::MessageDispatcher()
    {
    }

    void MessageDispatcher::sendMessage(Message& message)
    {
        for (const unsigned char &i : message.payload)
            std::cout << i << std::endl;
    }

}
