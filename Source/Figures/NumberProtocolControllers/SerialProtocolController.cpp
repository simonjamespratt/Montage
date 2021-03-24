#include "SerialProtocolController.h"

SerialProtocolController::SerialProtocolController()
{
    text.setText("No parameters to display", juce::dontSendNotification);
    addAndMakeVisible(&text);
}

void SerialProtocolController::resized()
{
    text.setBounds(getLocalBounds());
}

void SerialProtocolController::setParams(aleatoric::NumberProtocolParams params)
{}

aleatoric::NumberProtocolParams SerialProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(aleatoric::SerialParams());
}
