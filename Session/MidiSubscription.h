# pragma once
#include <vector>

#include "Messaging/MessageReceiver.h"

namespace AbletonProject
{
    /*
        Huge comment incoming:

        - Temporary hack if we can't pass instance of MessageReceiver in
        -Keeping a non-owning ptr, so be careful to unsubscribe. Could use shared_ptr, weak_ptr etc
        -Clearing global_MidiSubcriptions in the session destructor,because that's the scope I'm
        using this in
    */
    extern std::vector<MessageReceiver*> global_MidiSubcriptions;
    extern void(*global_MidiSubcriptionsLambda)(double, std::vector<unsigned char>*, void*);

    extern void global_subscribeForMidiCallback(MessageReceiver* receiver);
    extern void global_unsubscribeFromMidiCallback(MessageReceiver* receiver);
}