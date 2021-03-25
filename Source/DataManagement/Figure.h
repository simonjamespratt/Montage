#pragma once

#include <juce_data_structures/juce_data_structures.h>

class Figure {
  public:
    Figure();
    Figure(const juce::ValueTree &v);
    juce::Uuid getId() const;
    juce::ValueTree getState() const;

  private:
    juce::ValueTree state;
};
