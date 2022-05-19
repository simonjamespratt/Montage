#pragma once

#include "DurationProtocolParams.h"

#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class DurationProtocolController : public juce::Component {
  public:
    virtual ~DurationProtocolController() = default;

    // Factories
    static std::unique_ptr<DurationProtocolController>
    create(DurationProtocolType type, DurationProtocolParams &params);

    static std::unique_ptr<aleatoric::DurationProtocol>
    createProtocol(DurationProtocolParams &params);
};
