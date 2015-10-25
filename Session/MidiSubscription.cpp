#include "MidiSubscription.h"

namespace AbletonProject
{
    std::vector<MessageReceiver*> global_MidiSubcriptions;

    void(*global_MidiSubcriptionsLambda)(double, std::vector<unsigned char>*, void*) =
        [](double deltatime, std::vector<unsigned char> *rawMessage, void *userData)
    {
        for (MessageReceiver*& subscribingReceiver : global_MidiSubcriptions)
        {
            subscribingReceiver->receiveRawMidiMessage(deltatime, rawMessage, userData);
        }
    };

    void global_subscribeForMidiCallback(MessageReceiver* receiver)
    {
        global_MidiSubcriptions.push_back(receiver);
    }

    void global_unsubscribeFromMidiCallback(MessageReceiver* receiver)
    {
        global_MidiSubcriptions.erase(
            std::remove(
            global_MidiSubcriptions.begin(), global_MidiSubcriptions.end(), receiver),
            global_MidiSubcriptions.end()
            );
    }
}