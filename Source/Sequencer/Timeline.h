#pragma once
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class Scale {
  public:
    Scale(float i, juce::Rectangle<int> a);
    virtual ~Scale() = default;
    void drawScale(double editLength, juce::Graphics &g);

  protected:
    enum class LineSize { big, medium, small };
    juce::Rectangle<int> area;
    juce::Rectangle<float> getLineMarker(float x, LineSize size);
    void drawNumber(float timePoint, float x, juce::Graphics &g);

  private:
    float increment;
    virtual void performLayout(float timePoint, juce::Graphics &g, float x) = 0;
};

class GranularScale : public Scale {
  public:
    GranularScale(juce::Rectangle<int> a);
    void performLayout(float timePoint, juce::Graphics &g, float x) override;
};

class MidScale : public Scale {
  public:
    MidScale(juce::Rectangle<int> a);
    void performLayout(float timePoint, juce::Graphics &g, float x) override;
};

class CoarseScale : public Scale {
  public:
    CoarseScale(juce::Rectangle<int> a);
    void performLayout(float timePoint, juce::Graphics &g, float x) override;
};

// ===========================================================

class Timeline : public juce::Component {
  public:
    Timeline(te::Edit &e);
    void paint(juce::Graphics &) override;
    void recalculate();

  private:
    te::Edit &edit;
    std::unique_ptr<Scale> scale;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};
