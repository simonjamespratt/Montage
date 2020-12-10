#pragma once

#include "Identifiers.h"
#include "Particle.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class ParticleCollection {
  public:
    ParticleCollection(const juce::ValueTree &v);
    const std::vector<Particle> getParticles();

  private:
    juce::ValueTree state;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticleCollection)
};
