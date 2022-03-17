#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class DurationProtocolSelector : public juce::Component {
  public:
    DurationProtocolSelector(DurationProtocolParams &durationParams);

    void resized() override;

  private:
    DurationProtocolParams &params;
    std::unique_ptr<DurationProtocolController> controller;

    juce::Label protocolSelectorLabel;
    juce::ComboBox protocolSelector;

    void configureProtocolSelector();
    void protocolChanged();
    void setInitialActiveProtocol();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DurationProtocolSelector)
};
