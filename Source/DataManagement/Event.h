#pragma once
#include "Figure.h"
#include "Particle.h"

#include <juce_data_structures/juce_data_structures.h>

class Event : public juce::ValueTree::Listener {
  public:
    Event(const juce::ValueTree &v, const Figure &f, const Particle &p);
    Event(const Figure &f, const Particle &p, double onset);
    ~Event();

    juce::Uuid getId() const;

    double getOnset() const;
    void setOnset(double newOnset);

    juce::Uuid getParticleId() const;
    Particle getParticle() const;

    juce::Uuid getFigureId() const;
    Figure getFigure() const;

    juce::ValueTree getState() const;

    std::function<void(juce::Identifier propertyChanged)> onUpdated;

    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override;

  private:
    juce::ValueTree state;
    Particle particle;
    Figure figure;

    void validateOnset(double newOnset);
};
