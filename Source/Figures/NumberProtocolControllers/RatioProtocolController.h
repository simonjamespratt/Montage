#pragma once

#include "NumberProtocolController.h"
#include "NumericCollectionEditor.h"

class RatioProtocolController : public NumberProtocolController {
  public:
    RatioProtocolController(aleatoric::NumberProtocolParams initialParams);
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;
    void resized() override;

  private:
    std::vector<int> ratios {};
    NumericCollectionEditor ratiosEditor;
    juce::Viewport ratiosEditorViewport;
};
