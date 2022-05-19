#include "MultiplesProtocolController.h"

MultiplesProtocolController::MultiplesProtocolController(
    DurationProtocolParams &p)
: params(p),
  baseIncrementEditor(params.multiples.baseIncrement, "Base increment"),
  rangeStartEditor(params.multiples.rangeStart, "Range start"),
  rangeEndEditor(params.multiples.rangeEnd, "Range end"),
  deviationFactorEditor(
      params.multiples.deviationFactor, "Deviation factor", 0, 1.0, "", 1, 50),
  multipliersEditor(params.multiples.multipliers)
{
    addAndMakeVisible(&baseIncrementEditor);
    addAndMakeVisible(&deviationFactorEditor);
    addAndMakeVisible(&rangeStartEditor);
    addAndMakeVisible(&rangeEndEditor);

    multipliersEditorViewport.setViewedComponent(&multipliersEditor, false);
    multipliersEditorViewport.setScrollBarsShown(true, false);
    addChildComponent(&multipliersEditorViewport);

    multipliersSelectionHeading.setText("Multipliers",
                                        juce::dontSendNotification);
    addAndMakeVisible(&multipliersSelectionHeading);

    multipliersByRangeButton.setRadioGroupId(multipliersRadioGroup);
    multipliersByHandButton.setRadioGroupId(multipliersRadioGroup);

    addAndMakeVisible(&multipliersByRangeButton);
    addAndMakeVisible(&multipliersByHandButton);

    multipliersByRangeButton.onClick = [this] {
        toggleMultiplierStrategy(&multipliersByRangeButton,
                                 MultiplierStrategy::range);
    };
    multipliersByHandButton.onClick = [this] {
        toggleMultiplierStrategy(&multipliersByHandButton,
                                 MultiplierStrategy::hand);
    };

    multipliersByRangeButton.setToggleState(true, juce::sendNotification);
}

void MultiplesProtocolController::resized()
{
    auto margin = 10;
    auto marginSmall = 5;
    auto area = getLocalBounds();

    // params
    auto paramsArea = area.removeFromLeft(250);
    baseIncrementEditor.setBounds(paramsArea.removeFromTop(45));
    deviationFactorEditor.setBounds(paramsArea.removeFromTop(45));

    multipliersEditorViewport.setBounds(paramsArea);
    multipliersEditor.setBounds(paramsArea);

    rangeStartEditor.setBounds(paramsArea.removeFromTop(45));
    rangeEndEditor.setBounds(paramsArea.removeFromTop(45));

    multipliersSelectionHeading.setBounds(
        area.removeFromTop(30).reduced(marginSmall));

    multipliersByRangeButton.setBounds(
        area.removeFromTop(30).reduced(marginSmall));
    multipliersByHandButton.setBounds(
        area.removeFromTop(30).reduced(marginSmall));
}

// Private methods
void MultiplesProtocolController::toggleMultiplierStrategy(
    juce::Button *button, MultiplierStrategy strategy)
{
    auto newState = button->getToggleState();
    auto isActive = newState;

    if(strategy == MultiplierStrategy::hand) {
        multipliersEditorViewport.setVisible(newState);
    }

    if(strategy == MultiplierStrategy::range) {
        rangeStartEditor.setVisible(newState);
        rangeEndEditor.setVisible(newState);
    }

    if(isActive) {
        params.multiples.strategy = strategy;
    }

    resized();
}
