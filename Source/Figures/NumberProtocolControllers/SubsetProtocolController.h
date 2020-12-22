#pragma once

#include "NumberProtocolController.h"

class SubsetProtocolController : public NumberProtocolController {
  public:
    SubsetProtocolController(aleatoric::NumberProtocolParams initialParams);
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;
    void resized() override;

  private:
    int min;
    int max;
    int rangeMax;

    juce::ComboBox minSelector;
    juce::Label minLabel;
    juce::ComboBox maxSelector;
    juce::Label maxLabel;

    void selectorChanged(juce::ComboBox &selector);
    void setSelectorItems();
};
