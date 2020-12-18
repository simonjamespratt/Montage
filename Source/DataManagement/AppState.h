#pragma once

#include <juce_data_structures/juce_data_structures.h>

class AppState {
  public:
    AppState(const juce::ValueTree &v);
    bool saveStateToFile(const juce::File &file);
    juce::ValueTree loadStateFromFile(const juce::File &file);

  private:
    juce::ValueTree state;
};
