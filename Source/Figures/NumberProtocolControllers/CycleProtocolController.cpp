#include "CycleProtocolController.h"

CycleProtocolController::CycleProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: bidirectionalToggle("Bidirectional"),
  reverseDirectionToggle("Reverse Direction")
{
    addAndMakeVisible(&bidirectionalToggle);
    addAndMakeVisible(&reverseDirectionToggle);
    bidirectionalToggle.onClick = [this] {
        updateState(bidirectionalToggle);
    };
    reverseDirectionToggle.onClick = [this] {
        updateState(reverseDirectionToggle);
    };

    setParams(initialParams);
}

CycleProtocolController::~CycleProtocolController()
{}

void CycleProtocolController::paint(juce::Graphics &g)
{}

void CycleProtocolController::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    bidirectionalToggle.setBounds(area.removeFromTop(45).reduced(margin));
    reverseDirectionToggle.setBounds(area.removeFromTop(45).reduced(margin));
}

void CycleProtocolController::setParams(aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::cycle);

    auto cylceParams = params.getCycle();

    isBidirectional = cylceParams.getBidirectional();
    bidirectionalToggle.setToggleState(isBidirectional,
                                       juce::dontSendNotification);

    isReverseDirection = cylceParams.getReverseDirection();
    reverseDirectionToggle.setToggleState(isReverseDirection,
                                          juce::dontSendNotification);
}

aleatoric::NumberProtocolParams CycleProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(
        aleatoric::CycleParams(isBidirectional, isReverseDirection));
}

// Private methods
void CycleProtocolController::updateState(juce::Button &button)
{
    auto buttonName = button.getName();
    auto state = button.getToggleState();

    if(buttonName == "Bidirectional") {
        isBidirectional = state;
    }

    if(buttonName == "Reverse Direction") {
        isReverseDirection = state;
    }

    notifyParamsChanged();
}
