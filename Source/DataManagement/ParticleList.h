#pragma once
#include "ObjectList.h"
#include "Particle.h"

#include <functional>
#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class ParticleList : public ObjectList<Particle> {
  public:
    ParticleList(juce::ValueTree vt);

  private:
    bool isSuitableType(const juce::ValueTree &vt) const override;
    Particle makeObject(const juce::ValueTree &vt) const override;
    juce::ValueTree getObjectState(const Particle &particle) const override;
    void validateObjectAddition(const Particle &particle) override;
    void validateObjectRemoval(const Particle &particle) override;
};
