#include "DurationProtocolSelector.h"

#include "DurationProtocolConfig.h"

DurationProtocolSelector::DurationProtocolSelector(
    std::shared_ptr<aleatoric::DurationsProducer> durationsProducer,
    DurationProtocolParams durationParams)
: producer(durationsProducer), params(durationParams)
{
    protocolSelectorLabel.setText("Durations strategy: ",
                                  juce::dontSendNotification);
    addAndMakeVisible(&protocolSelectorLabel);

    addAndMakeVisible(&protocolSelector);
    configureProtocolSelector();
    protocolSelector.onChange = [this] {
        protocolChanged();
    };
    setInitialActiveProtocol();
}

void DurationProtocolSelector::paint(juce::Graphics &g)
{}

void DurationProtocolSelector::resized()
{
    auto area = getLocalBounds();
    auto margin = 10;
    auto chooseProtocolArea = area.removeFromTop(45);
    auto protocolColWidth = chooseProtocolArea.getWidth() / 2;
    protocolSelectorLabel.setBounds(
        chooseProtocolArea.removeFromLeft(protocolColWidth).reduced(margin));
    protocolSelector.setBounds(chooseProtocolArea.reduced(margin));

    auto controlsArea = area;

    if(controller != nullptr) {
        controller->setBounds(controlsArea);
    }
}

// Private methods
void DurationProtocolSelector::configureProtocolSelector()
{
    for(auto &&config : DurationProtocolConfig::getConfigurations()) {
        protocolSelector.addItem(config.getName(), config.getId());
    }
}

void DurationProtocolSelector::protocolChanged()
{
    auto id = protocolSelector.getSelectedId();
    auto selectedConfig = DurationProtocolConfig::findById(id);
    controller =
        DurationProtocolController::create(selectedConfig.getProtocolType(),
                                           params,
                                           producer);
    addAndMakeVisible(*controller);
    resized();
}

void DurationProtocolSelector::setInitialActiveProtocol()
{
    auto config = DurationProtocolConfig::findByType(params.activeType);
    protocolSelector.setSelectedId(config.getId(), juce::dontSendNotification);
    protocolChanged();
}
