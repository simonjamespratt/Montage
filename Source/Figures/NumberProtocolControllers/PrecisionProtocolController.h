#pragma once

#include "NumberProtocolController.h"
#include "SliderWithLabel.h"

// NB: distribution values - which are edited by sliders - currently allow
// setting a value to a number in range of 0-1. There is no constraint in
// place to ensure that all values in the distribution always sum to 1.
// Aleatoric - and the underlying number generator - doesn't care about that,
// it just sees the number for each value in the distribution as proportional to
// the overall sum of all the values in the distribution. Therefore the concept
// of summing all values to 1 is purely cosmetic. A future iteration could
// introduce the constraint of summing all values to 1, but it may not even be
// that useful to the user. Further use will start to give a clearer picture of
// whether that would be useful. If it were implemented, I imagine it would work
// something like this:

// Given 3 values & user increses val_1 by n. val_2 and val_3 are each decreased
// by n/2. And vice versa

class PrecisionProtocolController : public NumberProtocolController {
  public:
    PrecisionProtocolController(aleatoric::NumberProtocolParams initialParams);
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    std::vector<double> distribution;
    std::vector<std::unique_ptr<SliderWithLabel>> editors;
    juce::Component editorsContainer;
    juce::Viewport editorsViewport;
};
