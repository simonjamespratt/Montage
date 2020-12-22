#include "GeometricProtocolController.h"

GeometricProtocolController::GeometricProtocolController(
    DurationProtocolParams &params,
    std::shared_ptr<aleatoric::DurationsProducer> producer)
: m_params(params),
  m_producer(producer),
  rangeStart(m_params.geometric.rangeStart, "Range start"),
  rangeEnd(m_params.geometric.rangeEnd, "Range end"),
  collectionSize(m_params.geometric.collectionSize, "Collection size")
{
    addAndMakeVisible(&rangeStart);
    addAndMakeVisible(&rangeEnd);
    addAndMakeVisible(&collectionSize);

    saveButton.setButtonText("Set protocol");
    saveButton.onClick = [this] {
        setProtocol();
    };
    addAndMakeVisible(&saveButton);

    errorMessage.setText("New protocol was not set. The collection size needs "
                         "to be 2 or greater",
                         juce::dontSendNotification);
    // TODO: GENERAL-UI: ErrorMessage: see PrescribedProtocolController
    errorMessage.setColour(juce::Label::outlineColourId,
                           juce::Colours::orangered);
    errorMessage.setColour(juce::Label::textColourId, juce::Colours::orangered);
    addChildComponent(&errorMessage);
}

void GeometricProtocolController::paint(juce::Graphics &g)
{}

void GeometricProtocolController::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();

    // Params controllers
    auto paramsArea = area.removeFromLeft(250);

    if(errorMessage.isVisible()) {
        errorMessage.setBounds(paramsArea.removeFromTop(80).reduced(margin));
    }

    rangeStart.setBounds(paramsArea.removeFromTop(45));
    rangeEnd.setBounds(paramsArea.removeFromTop(45));
    collectionSize.setBounds(paramsArea.removeFromTop(45));

    // Save
    saveButton.setBounds(area.removeFromTop(45).reduced(margin));
}

// Private methods
void GeometricProtocolController::setProtocol()
{
    auto &params = m_params.geometric;
    try {
        m_producer->setDurationProtocol(
            aleatoric::DurationProtocol::createGeometric(
                aleatoric::Range(params.rangeStart, params.rangeEnd),
                params.collectionSize));

        if(errorMessage.isVisible()) {
            errorMessage.setVisible(false);
            resized();
        }

    } catch(const std::invalid_argument &e) {
        errorMessage.setVisible(true);
        resized();
    }
}
