#pragma once

#include "Event.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class Figure {
  public:
    Figure(const juce::ValueTree &v);
    int getId();
    std::vector<Event> getEvents();
    Event createEvent(double onset, int particleId);

  private:
    juce::ValueTree state;
};
