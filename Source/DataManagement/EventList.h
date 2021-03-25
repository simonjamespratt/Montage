#pragma once
#include "Event.h"
#include "ObjectList.h"

class EventList : public ObjectList<Event> {
  public:
    EventList(juce::ValueTree vt);
    EventList(juce::ValueTree vt, const Figure &f);

  private:
    bool isSuitableType(const juce::ValueTree &vt) const override;
    Event makeObject(const juce::ValueTree &vt) const override;
    juce::ValueTree getObjectState(const Event &particle) const override;
    void validateObjectAddition(const Event &event) override;
    void validateObjectRemoval(const Event &event) override;

    std::shared_ptr<Figure> figure;
};
