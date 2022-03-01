#pragma once

#include "Figure.h"
#include "Identifiers.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class SequencerViewState {
  public:
    SequencerViewState(te::Edit &e, Figure &f)
    : edit(e), state(IDs::SEQUENCER_VIEW_STATE)
    {
        state.setProperty(IDs::viewStateFigureName, f.getName(), nullptr);
        figureName.referTo(state, IDs::viewStateFigureName, nullptr, "");

        state.setProperty(IDs::sequencerLoaded, false, nullptr);
        sequencerIsLoaded.referTo(state, IDs::sequencerLoaded, nullptr);

        state.setProperty(IDs::trackHeight, 75, nullptr);
        state.setProperty(IDs::trackHeightMin, 20, nullptr);
        state.setProperty(IDs::trackHeightMax, 300, nullptr);

        trackHeight.referTo(state, IDs::trackHeight, nullptr);
        trackHeightMin.referTo(state, IDs::trackHeightMin, nullptr);
        trackHeightMax.referTo(state, IDs::trackHeightMax, nullptr);

        state.setProperty(IDs::panelsWidth, 0.2, nullptr);
        state.setProperty(IDs::panelsWidthMin, 0.05, nullptr);
        state.setProperty(IDs::panelsWidthMax, 0.4, nullptr);

        panelsWidth.referTo(state, IDs::panelsWidth, nullptr);
        panelsWidthMin.referTo(state, IDs::panelsWidthMin, nullptr);
        panelsWidthMax.referTo(state, IDs::panelsWidthMax, nullptr);

        state.setProperty(IDs::timeScalingFactorInitial, 100, nullptr);
        state.setProperty(IDs::timeScalingFactorCurrent, 100, nullptr);
        state.setProperty(IDs::timeScalingFactorMin, 0, nullptr);
        state.setProperty(IDs::timeScalingFactorMax, 1000, nullptr);

        timeScalingFactorInitial.referTo(state,
                                         IDs::timeScalingFactorInitial,
                                         nullptr);
        timeScalingFactorCurrent.referTo(state,
                                         IDs::timeScalingFactorCurrent,
                                         nullptr);
        timeScalingFactorMin.referTo(state, IDs::timeScalingFactorMin, nullptr);
        timeScalingFactorMax.referTo(state, IDs::timeScalingFactorMax, nullptr);

        state.setProperty(IDs::viewportSyncToMouseRequired, false, nullptr);
        viewportSyncToMouseRequired.referTo(state,
                                            IDs::viewportSyncToMouseRequired,
                                            nullptr);

        state.setProperty(IDs::viewportSyncToTransportRequired, false, nullptr);
        viewportSyncToTransportRequired.referTo(
            state,
            IDs::viewportSyncToTransportRequired,
            nullptr);
    }

    te::Edit &edit;
    juce::ValueTree state;

    juce::CachedValue<juce::String> figureName;
    juce::CachedValue<bool> sequencerIsLoaded;

    juce::CachedValue<double> trackHeight;
    juce::CachedValue<double> trackHeightMin;
    juce::CachedValue<double> trackHeightMax;

    juce::CachedValue<double> panelsWidth;
    juce::CachedValue<double> panelsWidthMin;
    juce::CachedValue<double> panelsWidthMax;

    juce::CachedValue<double> timeScalingFactorInitial;
    juce::CachedValue<double> timeScalingFactorCurrent;
    juce::CachedValue<double> timeScalingFactorMin;
    juce::CachedValue<double> timeScalingFactorMax;

    juce::CachedValue<bool> viewportSyncToMouseRequired;
    juce::CachedValue<bool> viewportSyncToTransportRequired;
};
