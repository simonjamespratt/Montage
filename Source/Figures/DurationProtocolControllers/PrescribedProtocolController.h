#pragma once
#include "DurationProtocolController.h"
#include "DurationProtocolParams.h"
#include "NumericCollectionEditor.h"

#include <memory>

class PrescribedProtocolController : public DurationProtocolController {
  public:
    PrescribedProtocolController(
        DurationProtocolParams &params,
        std::shared_ptr<aleatoric::DurationsProducer> producer);

    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    void setProtocol() override;

    DurationProtocolParams &m_params;
    std::shared_ptr<aleatoric::DurationsProducer> m_producer;

    NumericCollectionEditor durationsEditor;
    juce::Viewport viewport;

    juce::TextButton saveButton;
    juce::Label errorMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrescribedProtocolController)
};
