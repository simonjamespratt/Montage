#pragma once

#include "DurationProtocolParams.h"
#include "DurationProtocolSelector.h"
#include "NumberProtocolSelector.h"

#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class FigureOnsetSelection : public juce::Component {
  public:
    FigureOnsetSelection(
        std::shared_ptr<aleatoric::DurationsProducer> durationsProducer,
        DurationProtocolParams durationProtocolParams);
    ~FigureOnsetSelection();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    juce::Label heading;
    NumberProtocolSelector numberProtocolSelector;
    DurationProtocolSelector durationProtocolSelector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureOnsetSelection)
};
