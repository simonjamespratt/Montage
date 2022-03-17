#pragma once

#include "NumberProtocolController.h"
#include "Particle.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class NumberProtocolSelector : public juce::Component {
  public:
    NumberProtocolSelector(
        std::shared_ptr<aleatoric::CollectionsProducer<Particle>>
            particleProducer);
    NumberProtocolSelector(
        std::shared_ptr<aleatoric::DurationsProducer> durationsProducer);

    void resized() override;

    void updateParams();

  private:
    std::shared_ptr<aleatoric::CollectionsProducer<Particle>>
        m_particleProducer;
    std::shared_ptr<aleatoric::DurationsProducer> m_durationsProducer;

    std::unique_ptr<NumberProtocolController> controller;

    juce::Label protocolSelectorLabel;
    juce::ComboBox protocolSelector;

    void initialise();
    void configureProtocolSelector();
    void protocolChanged();
    void setInitialActiveProtocol();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberProtocolSelector)
};
