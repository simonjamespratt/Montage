#pragma once
#include "ObjectList.h"
#include "Source.h"

#include <functional>
#include <juce_data_structures/juce_data_structures.h>
#include <vector>

class SourceList : public ObjectList<Source> {
  public:
    SourceList(juce::ValueTree vt);

  private:
    bool isSuitableType(const juce::ValueTree &vt) const override;
    Source makeObject(const juce::ValueTree &vt) const override;
    juce::ValueTree getObjectState(const Source &source) const override;
    void validateObjectRemoval(const Source &source) override;
    void validateObjectAddition(const Source &source) override;
};
