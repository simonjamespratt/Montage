#pragma once

#include <juce_data_structures/juce_data_structures.h>

class Event {
  public:
    Event(const juce::ValueTree &v);
    int getId();
    double getOnset();
    int getParticleId();

  private:
    juce::ValueTree state;
};
