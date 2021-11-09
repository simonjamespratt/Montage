#pragma once
#include "Identifiers.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class StateService {
  public:
    static void checkTypeIsValid(const juce::ValueTree &state,
                                 juce::Identifier identifier);

    static void
    checkPropsAreValid(const juce::ValueTree &state,
                       std::vector<juce::Identifier> compulsoryProps,
                       std::vector<juce::Identifier> optionalProps = {});

    static juce::ValueTree
    getSourceStateForObject(const juce::ValueTree &objectState,
                            const juce::ValueTree &stateToSearch);

    static juce::ValueTree
    getParticleStateForObject(const juce::ValueTree &objectState,
                              const juce::ValueTree &stateToSearch);

    static juce::ValueTree
    getFigureStateForObject(const juce::ValueTree &objectState,
                            const juce::ValueTree &stateToSearch);

  private:
    static juce::ValueTree getState(const juce::ValueTree &objState,
                                    juce::Identifier dependencyType,
                                    juce::Identifier idTypeToFind,
                                    const juce::ValueTree &stateToSearch);
};
