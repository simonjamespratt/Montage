#include "MultiplesProtocolController.h"

MultiplesProtocolController::MultiplesProtocolController(
    DurationProtocolParams &params,
    std::shared_ptr<aleatoric::DurationsProducer> producer)
: m_params(params),
  m_producer(producer),
  baseIncrementEditor(m_params.multiples.baseIncrement, "Base increment"),
  rangeStartEditor(m_params.multiples.rangeStart, "Range start"),
  rangeEndEditor(m_params.multiples.rangeEnd, "Range end"),
  deviationFactorEditor(m_params.multiples.deviationFactor,
                        "Deviation factor",
                        0,
                        1.0,
                        "",
                        1,
                        50),
  multipliersEditor(m_params.multiples.multipliers)
{
    addAndMakeVisible(&baseIncrementEditor);
    addAndMakeVisible(&deviationFactorEditor);
    addAndMakeVisible(&rangeStartEditor);
    addAndMakeVisible(&rangeEndEditor);

    multipliersEditorViewport.setViewedComponent(&multipliersEditor, false);
    multipliersEditorViewport.setScrollBarsShown(true, false);
    addChildComponent(&multipliersEditorViewport);

    saveButton.setButtonText("Set protocol");
    saveButton.onClick = [this] {
        setProtocol();
    };
    addAndMakeVisible(&saveButton);

    // TODO: GENERAL-UI: ErrorMessage: see PrescribedProtocolController
    errorMessage.setColour(juce::Label::outlineColourId,
                           juce::Colours::orangered);
    errorMessage.setColour(juce::Label::textColourId, juce::Colours::orangered);
    addChildComponent(&errorMessage);

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

void MultiplesProtocolController::paint(juce::Graphics &g)
{}

void MultiplesProtocolController::resized()
{
    auto margin = 10;
    auto marginSmall = 5;
    auto area = getLocalBounds();

    // params
    auto paramsArea = area.removeFromLeft(250);

    if(errorMessage.isVisible()) {
        errorMessage.setBounds(paramsArea.removeFromTop(80).reduced(margin));
    }

    baseIncrementEditor.setBounds(paramsArea.removeFromTop(45));
    deviationFactorEditor.setBounds(paramsArea.removeFromTop(45));

    multipliersEditorViewport.setBounds(paramsArea);
    multipliersEditor.setBounds(paramsArea);

    rangeStartEditor.setBounds(paramsArea.removeFromTop(45));
    rangeEndEditor.setBounds(paramsArea.removeFromTop(45));

    saveButton.setBounds(area.removeFromTop(45).reduced(margin));

    multipliersSelectionHeading.setBounds(
        area.removeFromTop(30).reduced(marginSmall));

    multipliersByRangeButton.setBounds(
        area.removeFromTop(30).reduced(marginSmall));
    multipliersByHandButton.setBounds(
        area.removeFromTop(30).reduced(marginSmall));
}

// Private methods
void MultiplesProtocolController::setProtocol()
{
    auto &params = m_params.multiples;
    DBG("base increment: " << params.baseIncrement);
    DBG("dev factor: " << params.deviationFactor);
    DBG("range start: " << params.rangeStart);
    DBG("range end: " << params.rangeEnd);
    for(auto &&i : params.multipliers) {
        DBG("multiplier value: " << i);
    }

    if(currentMultiplierStrategy == MultiplierStrategy::range) {
        DBG("range strategy is active");

        try {
            m_producer->setDurationProtocol(
                aleatoric::DurationProtocol::createMultiples(
                    params.baseIncrement,
                    aleatoric::Range(params.rangeStart, params.rangeEnd),
                    params.deviationFactor));

            if(errorMessage.isVisible()) {
                errorMessage.setVisible(false);
                resized();
            }

        } catch(const std::invalid_argument &e) {
            errorMessage.setText(
                "New protocol was not set. When using range based multipliers, "
                "the range must be 2 or greater",
                juce::dontSendNotification);
            errorMessage.setVisible(true);
            resized();
        }
    }

    if(currentMultiplierStrategy == MultiplierStrategy::hand) {
        DBG("hand strategy is active");

        try {
            m_producer->setDurationProtocol(
                aleatoric::DurationProtocol::createMultiples(
                    params.baseIncrement,
                    params.multipliers,
                    params.deviationFactor));

            if(errorMessage.isVisible()) {
                errorMessage.setVisible(false);
                resized();
            }
        } catch(const std::invalid_argument &e) {
            errorMessage.setText("New protocol was not set. When using hand "
                                 "crafted multipliers, you must provide 2 or "
                                 "more duration values",
                                 juce::dontSendNotification);
            errorMessage.setVisible(true);
            resized();
        }
    }
}

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
        currentMultiplierStrategy = strategy;
    }

    resized();
}
