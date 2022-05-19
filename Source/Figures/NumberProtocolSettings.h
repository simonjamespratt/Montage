#pragma once

#include "VariantConverters.h"

#include <NumberProtocolParameters.hpp>
#include <juce_data_structures/juce_data_structures.h>

enum class NumberProtocolType {
    adjacentSteps,
    basic,
    cycle,
    granularWalk,
    groupedRepetition,
    noRepetition,
    periodic,
    precision,
    ratio,
    serial,
    subset,
    walk,
    none
};

template<>
struct juce::VariantConverter<NumberProtocolType> {
    static NumberProtocolType fromVar(const juce::var &v)
    {
        return static_cast<NumberProtocolType>(int(v));
    }
    static juce::var toVar(const NumberProtocolType &t)
    {
        return static_cast<int>(t);
    }
};

enum class CreationContext { duration_selection, particle_selection, none };

template<>
struct juce::VariantConverter<CreationContext> {
    static CreationContext fromVar(const juce::var &v)
    {
        return static_cast<CreationContext>(int(v));
    }
    static juce::var toVar(const CreationContext &t)
    {
        return static_cast<int>(t);
    }
};

struct CycleSettings {
    juce::CachedValue<bool> bidirectional;
    juce::CachedValue<bool> reverseDirection;
};

struct GranularWalkSettings {
    juce::CachedValue<double> deviationFactor;
};

struct GroupedRepetitionSettings {
    juce::CachedValue<std::vector<int>> groupings;
};

struct PeriodicSettings {
    juce::CachedValue<double> chanceOfRepetition;
};

struct PrecisionSettings {
    juce::CachedValue<std::vector<double>> distribution;
};

struct RatioSettings {
    juce::CachedValue<std::vector<int>> ratios;
};

struct SubsetSettings {
    juce::CachedValue<int> min;
    juce::CachedValue<int> max;
};

struct WalkSettings {
    juce::CachedValue<int> maxStep;
};

class NumberProtocolSettings {
  public:
    NumberProtocolSettings(aleatoric::NumberProtocolParams &params,
                           CreationContext c = CreationContext::none);
    NumberProtocolSettings(const juce::ValueTree &v);
    NumberProtocolSettings(const NumberProtocolSettings &origin);
    juce::CachedValue<NumberProtocolType> activeType;
    juce::CachedValue<CreationContext> context;
    juce::ValueTree state;
    CycleSettings cycle;
    GranularWalkSettings granularWalk;
    GroupedRepetitionSettings groupedRepetition;
    PeriodicSettings periodic;
    PrecisionSettings precision;
    RatioSettings ratio;
    SubsetSettings subset;
    WalkSettings walk;

  private:
    void addStateProps();
    void bindValuesToState(juce::ValueTree &vt);
    NumberProtocolType
    convertActiveType(const aleatoric::NumberProtocol::Type &type);
};
