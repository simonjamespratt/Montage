#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericCollectionEditor.h"
#include "NumericValueEditorWithLabel.h"
#include "SliderWithLabel.h"

#include <memory>

class MultiplesProtocolController : public DurationProtocolController {
  public:
    MultiplesProtocolController(DurationProtocolParams &p);

    void resized() override;

  private:
    DurationProtocolParams &params;

    NumericValueEditorWithLabel baseIncrementEditor;
    SliderWithLabel deviationFactorEditor;

    NumericValueEditorWithLabel rangeStartEditor;
    NumericValueEditorWithLabel rangeEndEditor;

    NumericCollectionEditor multipliersEditor;

    juce::Viewport multipliersEditorViewport;

    juce::Label multipliersSelectionHeading;
    juce::ToggleButton multipliersByRangeButton {"By Range"};
    juce::ToggleButton multipliersByHandButton {"By hand"};
    int multipliersRadioGroup = 1001;
    void toggleMultiplierStrategy(juce::Button *button,
                                  MultiplierStrategy strategy);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiplesProtocolController)
};
