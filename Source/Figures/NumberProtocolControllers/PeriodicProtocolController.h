#pragma once

#include "NumberProtocolController.h"
#include "SliderWithLabel.h"

class PeriodicProtocolController : public NumberProtocolController {
  public:
    PeriodicProtocolController(aleatoric::NumberProtocolParams initialParams);
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    double chanceOfRepetition;
    SliderWithLabel chanceOfRepEditor;
};
