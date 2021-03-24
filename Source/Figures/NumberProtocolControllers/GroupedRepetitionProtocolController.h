#pragma once
#include "NumberProtocolController.h"
#include "NumericCollectionEditor.h"

class GroupedRepetitionProtocolController : public NumberProtocolController {
  public:
    GroupedRepetitionProtocolController(
        aleatoric::NumberProtocolParams initialParams);
    void resized() override;
    void setParams(aleatoric::NumberProtocolParams params) override;
    aleatoric::NumberProtocolParams getParams() override;

  private:
    std::vector<int> groupings {};
    NumericCollectionEditor groupingsEditor;
    juce::Viewport groupingsEditorViewport;
};
