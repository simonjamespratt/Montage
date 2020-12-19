#pragma once

#include <juce_data_structures/juce_data_structures.h>

class AppState {
  public:
    AppState(juce::ValueTree &v);
    bool saveStateToFile(const juce::File &file);
    bool loadStateFromFile(const juce::File &file);

  private:
    juce::ValueTree state;
};
