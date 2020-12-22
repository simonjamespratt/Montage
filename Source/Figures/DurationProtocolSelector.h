#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"

#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class DurationProtocolSelector : public juce::Component {
  public:
    DurationProtocolSelector(
        std::shared_ptr<aleatoric::DurationsProducer> durationsProducer,
        DurationProtocolParams durationParams);

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    std::shared_ptr<aleatoric::DurationsProducer> producer;
    DurationProtocolParams params;
    std::unique_ptr<DurationProtocolController> controller;

    juce::Label protocolSelectorLabel;
    juce::ComboBox protocolSelector;

    void configureProtocolSelector();
    void protocolChanged();
    void setInitialActiveProtocol();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DurationProtocolSelector)
};
