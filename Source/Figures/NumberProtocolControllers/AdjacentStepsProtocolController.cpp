#include "AdjacentStepsProtocolController.h"

AdjacentStepsProtocolController::AdjacentStepsProtocolController()
{
    text.setText("No parameters to display", juce::dontSendNotification);
    addAndMakeVisible(&text);
}

AdjacentStepsProtocolController::~AdjacentStepsProtocolController()
{}

void AdjacentStepsProtocolController::paint(juce::Graphics &g)
{}

void AdjacentStepsProtocolController::resized()
{
    auto area = getLocalBounds();
    text.setBounds(area);
}

void AdjacentStepsProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{}

aleatoric::NumberProtocolParams AdjacentStepsProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(aleatoric::AdjacentStepsParams());
}
