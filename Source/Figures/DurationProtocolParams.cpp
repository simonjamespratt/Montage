#include "DurationProtocolParams.h"

#include "Identifiers.h"

DurationProtocolParams::DurationProtocolParams(DurationProtocolType type)
: activeType(type) {};

// ================================================================
// ================================================================

DurationProtocolSettings::DurationProtocolSettings()
: state(IDs::DURATION_SETTINGS)
{
    addStateProps();

    activeType = DurationProtocolType::prescribed;

    geometric.rangeStart = 1;
    geometric.rangeEnd = 10;
    geometric.collectionSize = 5;

    prescribed.durations = std::vector<int> {1000, 2000};

    multiples.strategy = MultiplierStrategy::range;
    multiples.rangeStart = 1;
    multiples.rangeEnd = 10;
    multiples.baseIncrement = 100;
    multiples.deviationFactor = 0;
    multiples.multipliers = std::vector<int> {1000, 2000};
}

DurationProtocolSettings::DurationProtocolSettings(const juce::ValueTree &v)
: state(v)
{
    jassert(state.hasType(IDs::DURATION_SETTINGS));
    bindValuesToState(state);
    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);
        bindValuesToState(child);
    }
}

DurationProtocolSettings::DurationProtocolSettings(
    const DurationProtocolParams &params)
: state(IDs::DURATION_SETTINGS)
{
    addStateProps();

    activeType = params.activeType;

    geometric.rangeStart = params.geometric.rangeStart;
    geometric.rangeEnd = params.geometric.rangeEnd;
    geometric.collectionSize = params.geometric.collectionSize;

    prescribed.durations = params.prescribed.durations;

    multiples.strategy = params.multiples.strategy;
    multiples.rangeStart = params.multiples.rangeStart;
    multiples.rangeEnd = params.multiples.rangeEnd;
    multiples.multipliers = params.multiples.multipliers;
    multiples.baseIncrement = params.multiples.baseIncrement;
    multiples.deviationFactor = params.multiples.deviationFactor;
}

DurationProtocolSettings::DurationProtocolSettings(
    const DurationProtocolSettings &d)
: state(d.state)
{
    bindValuesToState(state);
    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);
        bindValuesToState(child);
    }
}

void DurationProtocolSettings::addStateProps()
{
    state.setProperty(IDs::active_type, juce::var(), nullptr);
    bindValuesToState(state);

    juce::ValueTree geoState(IDs::GEOMETRIC_SETTINGS);
    geoState.setProperty(IDs::geo_range_start, juce::var(), nullptr);
    geoState.setProperty(IDs::geo_range_end, juce::var(), nullptr);
    geoState.setProperty(IDs::geo_collection_size, juce::var(), nullptr);
    state.addChild(geoState, -1, nullptr);
    bindValuesToState(geoState);

    juce::ValueTree prescribedState(IDs::PRESCRIBED_SETTINGS);
    prescribedState.setProperty(IDs::prescribed_durations,
                                juce::var(juce::Array<juce::var>()),
                                nullptr);
    state.addChild(prescribedState, -1, nullptr);
    bindValuesToState(prescribedState);

    juce::ValueTree multiState(IDs::MULTIPLES_SETTINGS);
    multiState.setProperty(IDs::multi_strategy, juce::var(), nullptr);
    multiState.setProperty(IDs::multi_range_start, juce::var(), nullptr);
    multiState.setProperty(IDs::multi_range_end, juce::var(), nullptr);
    multiState.setProperty(IDs::multi_base_increment, juce::var(), nullptr);
    multiState.setProperty(IDs::multi_deviation_factor, juce::var(), nullptr);
    multiState.setProperty(IDs::multi_multipliers,
                           juce::var(juce::Array<juce::var>()),
                           nullptr);
    state.addChild(multiState, -1, nullptr);
    bindValuesToState(multiState);
}

void DurationProtocolSettings::bindValuesToState(juce::ValueTree &vt)
{
    if(vt.hasType(IDs::DURATION_SETTINGS)) {
        activeType.referTo(vt, IDs::active_type, nullptr);
    }

    if(vt.hasType(IDs::GEOMETRIC_SETTINGS)) {
        geometric.rangeStart.referTo(vt, IDs::geo_range_start, nullptr);
        geometric.rangeEnd.referTo(vt, IDs::geo_range_end, nullptr);
        geometric.collectionSize.referTo(vt, IDs::geo_collection_size, nullptr);
    }

    if(vt.hasType(IDs::PRESCRIBED_SETTINGS)) {
        prescribed.durations.referTo(vt, IDs::prescribed_durations, nullptr);
    }

    if(vt.hasType(IDs::MULTIPLES_SETTINGS)) {
        multiples.strategy.referTo(vt, IDs::multi_strategy, nullptr);
        multiples.rangeStart.referTo(vt, IDs::multi_range_start, nullptr);
        multiples.rangeEnd.referTo(vt, IDs::multi_range_end, nullptr);
        multiples.baseIncrement.referTo(vt, IDs::multi_base_increment, nullptr);
        multiples.deviationFactor.referTo(vt,
                                          IDs::multi_deviation_factor,
                                          nullptr);
        multiples.multipliers.referTo(vt, IDs::multi_multipliers, nullptr);
    }
}
