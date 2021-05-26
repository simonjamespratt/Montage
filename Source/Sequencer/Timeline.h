#pragma once
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class Timeline : public juce::Component {
  public:
    Timeline(te::Edit &e);

    void paint(juce::Graphics &) override;

    void recalculate();

  private:
    te::Edit &edit;
    enum class LineSize { big, medium, small };
    juce::Rectangle<float> getLineMarker(float x, LineSize size);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};
