#pragma once

#include "Identifiers.h"

#include <juce_data_structures/juce_data_structures.h>

class Particle {
  public:
    Particle(const juce::ValueTree &v);
    int getId() const;
    int getSourceId() const;
    double getStart() const;
    double getEnd() const;

  private:
    juce::ValueTree state;
    // TODO: DATA-MANAGEMENT: why does this cause an error when returning a
    // vector of class Particle from Particles::getParticles()?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Particle)
};
