#include "NoRepetitionProtocolController.h"

NoRepetitionProtocolController::NoRepetitionProtocolController()
{
    text.setText("No parameters to display", juce::dontSendNotification);
    addAndMakeVisible(&text);
}

void NoRepetitionProtocolController::resized()
{
    text.setBounds(getLocalBounds());
}

void NoRepetitionProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{}

aleatoric::NumberProtocolParams NoRepetitionProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(aleatoric::NoRepetitionParams());
}
