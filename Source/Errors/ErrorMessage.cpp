#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(juce::String messageReceived)
: message(messageReceived)
{}

ErrorMessage::~ErrorMessage()
{}

void ErrorMessage::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); // clear the background
    g.setColour(juce::Colours::red);
    g.setFont(14.0f);
    g.drawText(message, getLocalBounds(), juce::Justification::centred, true);
}
