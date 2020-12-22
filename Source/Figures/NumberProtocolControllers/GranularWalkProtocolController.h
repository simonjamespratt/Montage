#pragma once
#include "NumberProtocolController.h"
#include "SliderWithLabel.h"

class GranularWalkProtocolController : public NumberProtocolController {
  public:
    GranularWalkProtocolController(
        aleatoric::NumberProtocolParams initialParams);
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    double deviationFactor;
    SliderWithLabel devFactorEditor;
};
