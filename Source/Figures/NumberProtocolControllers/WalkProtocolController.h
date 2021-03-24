#pragma once

#include "NumberProtocolController.h"
#include "NumericValueEditorWithLabel.h"

class WalkProtocolController : public NumberProtocolController {
  public:
    WalkProtocolController(aleatoric::NumberProtocolParams initialParams);
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;
    void resized() override;

  private:
    int maxStep = 1;
    NumericValueEditorWithLabel maxStepEditor;
};
