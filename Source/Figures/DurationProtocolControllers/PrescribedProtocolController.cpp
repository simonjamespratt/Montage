#include "PrescribedProtocolController.h"

PrescribedProtocolController::PrescribedProtocolController(
    DurationProtocolParams &params,
    std::shared_ptr<aleatoric::DurationsProducer> producer)
: m_params(params),
  m_producer(producer),
  durationsEditor(m_params.prescribed.durations)
{
    viewport.setViewedComponent(&durationsEditor, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&viewport);

    saveButton.setButtonText("Set protocol");
    saveButton.onClick = [this] {
        setProtocol();
    };
    addAndMakeVisible(&saveButton);

    errorMessage.setText(
        "New protocol was not set. You must provide 2 or more duration values.",
        juce::dontSendNotification);
    // TODO: GENERAL-UI: ErrorMessage: This should be a component. Used
    // elsewhere and should be standardised. This differs from the error message
    // in a modal. This message is to be displayed "inline" so some renaming
    // required.
    errorMessage.setColour(juce::Label::outlineColourId,
                           juce::Colours::orangered);
    errorMessage.setColour(juce::Label::textColourId, juce::Colours::orangered);
    addChildComponent(&errorMessage);
}

void PrescribedProtocolController::paint(juce::Graphics &g)
{}

void PrescribedProtocolController::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();

    auto viewsArea = area.removeFromLeft(250);

    if(errorMessage.isVisible()) {
        errorMessage.setBounds(viewsArea.removeFromTop(80).reduced(margin));
    }

    viewport.setBounds(viewsArea);
    durationsEditor.setBounds(viewsArea);

    saveButton.setBounds(area.removeFromTop(45).reduced(margin));
}

// Private methods
void PrescribedProtocolController::setProtocol()
{
    for(auto &&i : m_params.prescribed.durations) {
        DBG("duration: " << i);
    }

    try {
        m_producer->setDurationProtocol(
            aleatoric::DurationProtocol::createPrescribed(
                m_params.prescribed.durations));

        if(errorMessage.isVisible()) {
            errorMessage.setVisible(false);
            resized();
        }
    } catch(const std::invalid_argument
                &e) // TODO: ALEATORIC: ideally this is an aleatoric error, so
                    // can be absolutely clear about what you're catching
    {
        errorMessage.setVisible(true);
        resized();
    }
}
