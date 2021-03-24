#pragma once
#include "NumberProtocolController.h"

class NoRepetitionProtocolController : public NumberProtocolController {
  public:
    NoRepetitionProtocolController();
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    juce::Label text;
};
