#include "NumberProtocolSettings.h"

#include "Identifiers.h"

NumberProtocolSettings::NumberProtocolSettings(
    aleatoric::NumberProtocolParams &params, CreationContext c)
: state(IDs::NUMBER_SETTINGS)
{
    addStateProps();

    activeType = convertActiveType(params.getActiveProtocol());
    context = c;

    auto cycleParams = params.getCycle();
    cycle.bidirectional = cycleParams.getBidirectional();
    cycle.reverseDirection = cycleParams.getReverseDirection();

    granularWalk.deviationFactor =
        params.getGranularWalk().getDeviationFactor();

    groupedRepetition.groupings = params.getGroupedRepetition().getGroupings();

    periodic.chanceOfRepetition = params.getPeriodic().getChanceOfRepetition();

    precision.distribution = params.getPrecision().getDistribution();

    ratio.ratios = params.getRatio().getRatios();

    auto subsetParams = params.getSubset();
    subset.min = subsetParams.getMin();
    subset.max = subsetParams.getMax();

    walk.maxStep = params.getWalk().getMaxStep();
}

NumberProtocolSettings::NumberProtocolSettings(const juce::ValueTree &v)
: state(v)
{
    jassert(state.hasType(IDs::NUMBER_SETTINGS));
    bindValuesToState(state);
    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);
        bindValuesToState(child);
    }
}

NumberProtocolSettings::NumberProtocolSettings(
    const NumberProtocolSettings &origin)
: state(origin.state)
{
    bindValuesToState(state);
    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);
        bindValuesToState(child);
    }
}

// Private methods
void NumberProtocolSettings::addStateProps()
{
    state.setProperty(IDs::active_type, juce::var(), nullptr);
    state.setProperty(IDs::number_settings_context, juce::var(), nullptr);
    bindValuesToState(state);

    juce::ValueTree cycleState(IDs::CYCLE_SETTINGS);
    cycleState.setProperty(IDs::cycle_bidirectional, juce::var(), nullptr);
    cycleState.setProperty(IDs::cycle_reverse_direction, juce::var(), nullptr);
    state.addChild(cycleState, -1, nullptr);
    bindValuesToState(cycleState);

    juce::ValueTree granWalkState(IDs::GRANULAR_WALK_SETTINGS);
    granWalkState.setProperty(IDs::granular_walk_deviation_factor,
                              juce::var(),
                              nullptr);
    state.addChild(granWalkState, -1, nullptr);
    bindValuesToState(granWalkState);

    juce::ValueTree groupedRepState(IDs::GROUPED_REPETITION_SETTINGS);
    groupedRepState.setProperty(IDs::grouped_repetition_groupings,
                                juce::var(juce::Array<juce::var>()),
                                nullptr);
    state.addChild(groupedRepState, -1, nullptr);
    bindValuesToState(groupedRepState);

    juce::ValueTree periodicState(IDs::PERIODIC_SETTINGS);
    periodicState.setProperty(IDs::periodic_chance_of_repetition,
                              juce::var(),
                              nullptr);
    state.addChild(periodicState, -1, nullptr);
    bindValuesToState(periodicState);

    juce::ValueTree precisionState(IDs::PRECISION_SETTINGS);
    precisionState.setProperty(IDs::precision_distribution,
                               juce::var(juce::Array<juce::var>()),
                               nullptr);
    state.addChild(precisionState, -1, nullptr);
    bindValuesToState(precisionState);

    juce::ValueTree ratioState(IDs::RATIO_SETTINGS);
    ratioState.setProperty(IDs::ratio_ratios,
                           juce::var(juce::Array<juce::var>()),
                           nullptr);
    state.addChild(ratioState, -1, nullptr);
    bindValuesToState(ratioState);

    juce::ValueTree subsetState(IDs::SUBSET_SETTINGS);
    subsetState.setProperty(IDs::subset_min, juce::var(), nullptr);
    subsetState.setProperty(IDs::subset_max, juce::var(), nullptr);
    state.addChild(subsetState, -1, nullptr);
    bindValuesToState(subsetState);

    juce::ValueTree walkState(IDs::WALK_SETTINGS);
    walkState.setProperty(IDs::walk_max_step, juce::var(), nullptr);
    state.addChild(walkState, -1, nullptr);
    bindValuesToState(walkState);
}

void NumberProtocolSettings::bindValuesToState(juce::ValueTree &vt)
{
    if(vt.hasType(IDs::NUMBER_SETTINGS)) {
        activeType.referTo(vt, IDs::active_type, nullptr);
        context.referTo(vt, IDs::number_settings_context, nullptr);
    }

    if(vt.hasType(IDs::CYCLE_SETTINGS)) {
        cycle.bidirectional.referTo(vt, IDs::cycle_bidirectional, nullptr);
        cycle.reverseDirection.referTo(vt,
                                       IDs::cycle_reverse_direction,
                                       nullptr);
    }

    if(vt.hasType(IDs::GRANULAR_WALK_SETTINGS)) {
        granularWalk.deviationFactor.referTo(
            vt,
            IDs::granular_walk_deviation_factor,
            nullptr);
    }

    if(vt.hasType(IDs::GROUPED_REPETITION_SETTINGS)) {
        groupedRepetition.groupings.referTo(vt,
                                            IDs::grouped_repetition_groupings,
                                            nullptr);
    }

    if(vt.hasType(IDs::PERIODIC_SETTINGS)) {
        periodic.chanceOfRepetition.referTo(vt,
                                            IDs::periodic_chance_of_repetition,
                                            nullptr);
    }

    if(vt.hasType(IDs::PRECISION_SETTINGS)) {
        precision.distribution.referTo(vt,
                                       IDs::precision_distribution,
                                       nullptr);
    }

    if(vt.hasType(IDs::RATIO_SETTINGS)) {
        ratio.ratios.referTo(vt, IDs::ratio_ratios, nullptr);
    }

    if(vt.hasType(IDs::SUBSET_SETTINGS)) {
        subset.min.referTo(vt, IDs::subset_min, nullptr);
        subset.max.referTo(vt, IDs::subset_max, nullptr);
    }

    if(vt.hasType(IDs::WALK_SETTINGS)) {
        walk.maxStep.referTo(vt, IDs::walk_max_step, nullptr);
    }
}

NumberProtocolType NumberProtocolSettings::convertActiveType(
    const aleatoric::NumberProtocol::Type &type)
{
    switch(type) {
    case aleatoric::NumberProtocol::Type::basic:
        return NumberProtocolType::basic;
    case aleatoric::NumberProtocol::Type::cycle:
        return NumberProtocolType::cycle;
    case aleatoric::NumberProtocol::Type::granularWalk:
        return NumberProtocolType::granularWalk;
    case aleatoric::NumberProtocol::Type::groupedRepetition:
        return NumberProtocolType::groupedRepetition;
    case aleatoric::NumberProtocol::Type::noRepetition:
        return NumberProtocolType::noRepetition;
    case aleatoric::NumberProtocol::Type::periodic:
        return NumberProtocolType::periodic;
    case aleatoric::NumberProtocol::Type::precision:
        return NumberProtocolType::precision;
    case aleatoric::NumberProtocol::Type::ratio:
        return NumberProtocolType::ratio;
    case aleatoric::NumberProtocol::Type::serial:
        return NumberProtocolType::serial;
    case aleatoric::NumberProtocol::Type::subset:
        return NumberProtocolType::subset;
    case aleatoric::NumberProtocol::Type::walk:
        return NumberProtocolType::walk;
    case aleatoric::NumberProtocol::Type::adjacentSteps:
        return NumberProtocolType::adjacentSteps;
    case aleatoric::NumberProtocol::Type::none:
        return NumberProtocolType::none;
    }
}
