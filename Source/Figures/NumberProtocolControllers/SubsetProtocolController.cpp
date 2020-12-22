#include "SubsetProtocolController.h"

SubsetProtocolController::SubsetProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: minSelector("min_selector"), maxSelector("max_selector")
{
    setParams(initialParams);

    minSelector.onChange = [this] {
        selectorChanged(minSelector);
    };
    addAndMakeVisible(&minSelector);

    minLabel.setText("Min: ", juce::dontSendNotification);
    addAndMakeVisible(&minLabel);

    maxSelector.onChange = [this] {
        selectorChanged(maxSelector);
    };
    addAndMakeVisible(&maxSelector);

    maxLabel.setText("Max: ", juce::dontSendNotification);
    addAndMakeVisible(&maxLabel);
}

void SubsetProtocolController::setParams(aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::subset);

    auto subsetParams = params.getSubset();
    min = subsetParams.getMin();
    max = subsetParams.getMax();
    rangeMax = max;

    setSelectorItems();
}

aleatoric::NumberProtocolParams SubsetProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(aleatoric::SubsetParams(min, max));
}

void SubsetProtocolController::resized()
{
    auto area = getLocalBounds();
    auto margin = 10;

    auto minArea = area.removeFromTop(45);
    minLabel.setBounds(minArea.removeFromLeft(200).reduced(margin));
    minSelector.setBounds(minArea.removeFromLeft(100).reduced(margin));

    auto maxArea = area.removeFromTop(45);
    maxLabel.setBounds(maxArea.removeFromLeft(200).reduced(margin));
    maxSelector.setBounds(maxArea.removeFromLeft(100).reduced(margin));
}

// Private methods
void SubsetProtocolController::selectorChanged(juce::ComboBox &selector)
{
    auto selectorName = selector.getName();
    auto id = selector.getSelectedId();

    if(selectorName == "min_selector" && min != id) {
        min = id;
        notifyParamsChanged();
        setSelectorItems();
    }
    if(selectorName == "max_selector" && max != id) {
        max = id;
        notifyParamsChanged();
        setSelectorItems();
    }
}

void SubsetProtocolController::setSelectorItems()
{
    minSelector.clear();
    for(int i = 1; i <= max; i++) {
        minSelector.addItem(juce::String(i), i);
    }

    maxSelector.clear();
    for(int i = min; i <= rangeMax; i++) {
        maxSelector.addItem(juce::String(i), i);
    }

    minSelector.setSelectedId(min, juce::dontSendNotification);
    maxSelector.setSelectedId(max, juce::dontSendNotification);
}
