#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericValueEditorWithLabel.h"

#include <memory>

class GeometricProtocolController : public DurationProtocolController {
  public:
    GeometricProtocolController(DurationProtocolParams &p);

    void resized() override;

  private:
    DurationProtocolParams &params;

    NumericValueEditorWithLabel rangeStart;
    NumericValueEditorWithLabel rangeEnd;
    NumericValueEditorWithLabel collectionSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeometricProtocolController)
};
