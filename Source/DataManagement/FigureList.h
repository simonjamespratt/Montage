#pragma once
#include "Figure.h"
#include "ObjectList.h"

class FigureList : public ObjectList<Figure> {
  public:
    FigureList(juce::ValueTree vt);

  private:
    bool isSuitableType(const juce::ValueTree &vt) const override;
    Figure makeObject(const juce::ValueTree &vt) const override;
    juce::ValueTree getObjectState(const Figure &figure) const override;
    void validateObjectRemoval(const Figure &figure) override;
};
