// TODO: DURATION-PROTOCOL-PARAMS: There are currently two almost identical
// classes - DurationProtocolParams and DurationProtocolSettings - which should
// really be one class. The first is used for collecting and editing params that
// are then used to set an aleatoric producer and the latter is used to save
// the params (settings) within the figure they were used to create. Ideally the
// latter class would be used for both purposes. The reason for not doing so is
// that DurationProtocolSettings uses a value tree with cached values. To make
// inputs (Label, Slider etc.) set cached values correctly requires binding the
// values to the inputs which is a different pattern from that which is
// currently (see NumericEditorWithLabel etc.) being used. It would require
// refactoring (or making new components) to make inputs handle setting and
// getting from cached values.

#pragma once

#include "VariantConverters.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

enum class DurationProtocolType { geometric, multiples, prescribed };

template<>
struct juce::VariantConverter<DurationProtocolType> {
    static DurationProtocolType fromVar(const juce::var &v)
    {
        return static_cast<DurationProtocolType>(int(v));
    }
    static juce::var toVar(const DurationProtocolType &t)
    {
        return static_cast<int>(t);
    }
};

enum class MultiplierStrategy { range, hand };

template<>
struct juce::VariantConverter<MultiplierStrategy> {
    static MultiplierStrategy fromVar(const juce::var &v)
    {
        return static_cast<MultiplierStrategy>(int(v));
    }
    static juce::var toVar(const MultiplierStrategy &t)
    {
        return static_cast<int>(t);
    }
};

// ================================================================
// ================================================================

struct PrescribedProtocolParams {
    std::vector<int> durations {1000, 2000};
};

struct GeometricProtocolParams {
    int rangeStart = 1;
    int rangeEnd = 10;
    int collectionSize = 5;
};

struct MultiplesProtocolParams {
    MultiplierStrategy strategy {MultiplierStrategy::range};
    int rangeStart = 1;
    int rangeEnd = 10;
    std::vector<int> multipliers = {1000, 2000};
    int baseIncrement = 100;
    double deviationFactor = 0;
};

struct DurationProtocolParams {
    DurationProtocolParams(DurationProtocolType type);
    DurationProtocolType activeType;
    PrescribedProtocolParams prescribed;
    GeometricProtocolParams geometric;
    MultiplesProtocolParams multiples;
};

// ================================================================
// ================================================================

struct GeometricProtocolSettings {
    juce::CachedValue<int> rangeStart;
    juce::CachedValue<int> rangeEnd;
    juce::CachedValue<int> collectionSize;
};

struct PrescribedProtocolSettings {
    juce::CachedValue<std::vector<int>> durations;
};

struct MultiplesProtocolSettings {
    juce::CachedValue<MultiplierStrategy> strategy;
    juce::CachedValue<int> baseIncrement;
    juce::CachedValue<int> rangeStart;
    juce::CachedValue<int> rangeEnd;
    juce::CachedValue<std::vector<int>> multipliers;
    juce::CachedValue<double> deviationFactor;
};

class DurationProtocolSettings {
  public:
    DurationProtocolSettings();
    DurationProtocolSettings(const juce::ValueTree &v);
    DurationProtocolSettings(const DurationProtocolParams &params);
    DurationProtocolSettings(const DurationProtocolSettings &d);
    juce::CachedValue<DurationProtocolType> activeType;
    juce::ValueTree state;
    GeometricProtocolSettings geometric;
    PrescribedProtocolSettings prescribed;
    MultiplesProtocolSettings multiples;

  private:
    void addStateProps();
    void bindValuesToState(juce::ValueTree &vt);
};
