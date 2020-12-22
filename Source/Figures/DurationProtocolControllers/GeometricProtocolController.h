#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericValueEditorWithLabel.h"

#include <memory>

class GeometricProtocolController : public DurationProtocolController {
  public:
    GeometricProtocolController(
        DurationProtocolParams &params,
        std::shared_ptr<aleatoric::DurationsProducer> producer);
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    void setProtocol() override;

    DurationProtocolParams &m_params;
    std::shared_ptr<aleatoric::DurationsProducer> m_producer;

    NumericValueEditorWithLabel rangeStart;
    NumericValueEditorWithLabel rangeEnd;
    NumericValueEditorWithLabel collectionSize;
    juce::TextButton saveButton;
    juce::Label errorMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeometricProtocolController)
};
