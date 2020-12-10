#pragma once
#include "Identifiers.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class StateChecker {
  public:
    static void checkTypeIsValid(const juce::ValueTree &state,
                                 juce::Identifier identifier);

    static void checkChildTypesAreValid(const juce::ValueTree &state,
                                        juce::Identifier identifier);

    static void
    checkPropsAreValid(const juce::ValueTree &state,
                       std::vector<juce::Identifier> compulsoryProps,
                       std::vector<juce::Identifier> optionalProps = {});
};
