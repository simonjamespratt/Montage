#pragma once

#include "NumberProtocolController.h"

class SerialProtocolController : public NumberProtocolController {
  public:
    SerialProtocolController();
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    juce::Label text;
};
