#pragma once

#include "Figure.h"
#include "Identifiers.h"

#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class FigureCollection {
  public:
    FigureCollection(const juce::ValueTree &v);
    const std::vector<Figure> getFigures();
    Figure createFigure();

  private:
    juce::ValueTree state;
};
