#pragma once

#include <juce_data_structures/juce_data_structures.h>

class StateHelpers {
  public:
    static juce::ValueTree getParticleState(int id);
    static juce::ValueTree getFigureState(int id);
    static juce::ValueTree getEventState(int id);
};
