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

    // Factory
    static std::unique_ptr<DurationProtocolController>
    create(Type type,
           DurationProtocolParams &params,
           std::shared_ptr<aleatoric::DurationsProducer> producer);

  private:
    virtual void setProtocol() = 0;
};
