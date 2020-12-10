#pragma once

#include <juce_data_structures/juce_data_structures.h>

class CollectionHelper {
  public:
    static int createIdForNewChild(const juce::ValueTree &state);
};
