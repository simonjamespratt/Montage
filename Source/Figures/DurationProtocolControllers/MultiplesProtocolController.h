#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericCollectionEditor.h"
#include "NumericValueEditorWithLabel.h"
#include "SliderWithLabel.h"

#include <memory>

class MultiplesProtocolController : public DurationProtocolController {
  public:
    MultiplesProtocolController(
        DurationProtocolParams &params,
        std::shared_ptr<aleatoric::DurationsProducer> producer);
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    void setProtocol() override;

    DurationProtocolParams &m_params;
    std::shared_ptr<aleatoric::DurationsProducer> m_producer;

    NumericValueEditorWithLabel baseIncrementEditor;
    SliderWithLabel deviationFactorEditor;

    enum MultiplierStrategy { range, hand };
    MultiplierStrategy currentMultiplierStrategy {MultiplierStrategy::range};

    NumericValueEditorWithLabel rangeStartEditor;
    NumericValueEditorWithLabel rangeEndEditor;

    NumericCollectionEditor multipliersEditor;
    juce::Viewport multipliersEditorViewport;

    juce::TextButton saveButton;
    juce::Label errorMessage;

    juce::Label multipliersSelectionHeading;
    juce::ToggleButton multipliersByRangeButton {"By Range"};
    juce::ToggleButton multipliersByHandButton {"By hand"};
    int multipliersRadioGroup = 1001;
    void toggleMultiplierStrategy(juce::Button *button,
                                  MultiplierStrategy strategy);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiplesProtocolController)
};
