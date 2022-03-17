#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericCollectionEditor.h"

#include <memory>

class PrescribedProtocolController : public DurationProtocolController {
  public:
    PrescribedProtocolController(DurationProtocolParams &p);

    void resized() override;

  private:
    DurationProtocolParams &params;

    NumericCollectionEditor durationsEditor;
    juce::Viewport viewport;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrescribedProtocolController)
};
