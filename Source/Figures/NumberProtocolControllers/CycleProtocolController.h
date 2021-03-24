#pragma once
#include "NumberProtocolController.h"

class CycleProtocolController : public NumberProtocolController {
  public:
    CycleProtocolController(aleatoric::NumberProtocolParams initialParams);

    ~CycleProtocolController();

    void paint(juce::Graphics &g) override;

    void resized() override;

    void setParams(aleatoric::NumberProtocolParams params) override;

    aleatoric::NumberProtocolParams getParams() override;

  private:
    bool isBidirectional = false;
    bool isReverseDirection = false;
    juce::ToggleButton bidirectionalToggle;
    juce::ToggleButton reverseDirectionToggle;

    void updateState(juce::Button &button);
};
