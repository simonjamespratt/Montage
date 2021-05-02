#pragma once
#include "EventList.h"
#include "FigureList.h"
#include "ParticleList.h"
#include "SourceList.h"

#include <juce_data_structures/juce_data_structures.h>

class ProjectState {
  public:
    ProjectState(juce::ValueTree vt);
    ParticleList getParticleList() const;
    SourceList getSourceList() const;
    FigureList getFigureList() const;
    EventList getEventList() const;
    EventList getEventList(const Figure &f) const;

  private:
    juce::ValueTree state;
};
