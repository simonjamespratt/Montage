#pragma once

// NB: forward declaration to avoid circular dependency. Including
// DurationProtoolParams.h here results in a compilation error

class DurationProtocolParams;

#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class DurationProtocolController : public juce::Component {
  public:
    virtual ~DurationProtocolController() = default;

    enum class Type { geometric, multiples, prescribed };

    // Factories
    static std::unique_ptr<DurationProtocolController>
    create(Type type, DurationProtocolParams &params);

    static std::unique_ptr<aleatoric::DurationProtocol>
    createProtocol(DurationProtocolParams &params);
};
