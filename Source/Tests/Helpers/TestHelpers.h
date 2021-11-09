#pragma once

#include <juce_data_structures/juce_data_structures.h>

class StateHelpers {
  public:
    // Particle
    static juce::ValueTree createParticleState();

    static juce::ValueTree createParticleState(juce::Uuid id,
                                               juce::Uuid sourceId,
                                               double start = 0.0,
                                               double end = 1.0,
                                               juce::String name = "name");

    // Source
    static juce::ValueTree createSourceState();

    static juce::ValueTree createSourceState(juce::Uuid id);

    static juce::ValueTree createSourceState(juce::Uuid id,
                                             juce::String filePath);

    // Figure
    static juce::ValueTree createFigureState();
    static juce::ValueTree createFigureState(juce::Uuid id);

    // Event
    static juce::ValueTree createEventState();
    static juce::ValueTree createEventState(juce::Uuid id);

    static juce::ValueTree createEventState(juce::Uuid id,
                                            double onset,
                                            juce::Uuid particleId,
                                            juce::Uuid figureId);

    // Project state
    static juce::ValueTree createProjectState(int noOfSources,
                                              int noOfParticles,
                                              int noOfFigures,
                                              int noOfEvents);
};

class FileHelpers {
  public:
    static juce::String getTestFileName();
};
