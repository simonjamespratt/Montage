#pragma once

#include "Identifiers.h"
#include "Source.h"

#include <functional>
#include <juce_data_structures/juce_data_structures.h>

class Particle : public juce::ValueTree::Listener {
  public:
    Particle(const juce::ValueTree &v, const Source &s);
    Particle(const Source &s);
    Particle(const Particle &p);

    ~Particle();

    juce::Uuid getId() const;

    juce::Uuid getSourceId() const;
    Source getSource() const;

    double getStart() const;
    void setStart(double newStart);
    double ensureNewStartIsWithinBounds(const double &proposedStart,
                                        const double &existingEnd) const;

    double getEnd() const;
    void setEnd(double newEnd);
    double ensureNewEndIsWithinBounds(const double &existingStart,
                                      const double &proposedEnd) const;

    void setStartAndEnd(double newStart, double newEnd);

    juce::String getName() const;
    void setName(juce::String newName);

    juce::ValueTree getState() const;

    std::function<void(juce::Identifier propertyChanged)> onUpdated;

    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override;

  private:
    juce::ValueTree state;
    Source source;

    void validateStart(double start, double end);
    void validateEnd(double start, double end, double sourceFileLength);
};
