#pragma once

#include "NumberProtocolController.h"

class BasicProtocolController : public NumberProtocolController {
  public:
    BasicProtocolController();

    ~BasicProtocolController();

    void paint(juce::Graphics &g) override;

    void resized() override;

    void setParams(aleatoric::NumberProtocolParams params) override;

    aleatoric::NumberProtocolParams getParams() override;

  private:
    juce::Label text;
};
