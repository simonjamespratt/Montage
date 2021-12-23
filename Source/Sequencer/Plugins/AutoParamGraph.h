#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class AutoParamGraph : public juce::Component,
                       private te::AutomatableParameter::Listener {
  public:
    AutoParamGraph(te::AutomatableParameter::Ptr ap);
    ~AutoParamGraph();
    void paint(juce::Graphics &g) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseMove(const juce::MouseEvent &event) override;
    bool hitTest(int x, int y) override;

  private:
    struct PointData {
        te::AutomationCurve::AutomationPoint point;
        juce::Point<float> displayPoint;
        juce::Rectangle<float> displayPointArea;
    };

    struct TimeAndValue {
        double time;
        float value;
    };

    void curveHasChanged(te::AutomatableParameter &) override;

    juce::Point<float> convertAutomationPointToDisplayPoint(
        const te::AutomationCurve::AutomationPoint &point);

    TimeAndValue
    convertDisplayPointToTimeAndValue(const juce::Point<float> &point);

    int getPointIndexFromMouseEvent(const juce::MouseEvent &e);

    void handleTooltip(const juce::MouseEvent &e);

    te::AutomatableParameter::Ptr autoParam;
    std::vector<PointData> points;
    std::unique_ptr<int> indexOfMovingPoint;
    std::unique_ptr<juce::BubbleMessageComponent> tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutoParamGraph)
};
