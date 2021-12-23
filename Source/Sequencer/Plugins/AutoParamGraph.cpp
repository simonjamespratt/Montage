#include "AutoParamGraph.h"

AutoParamGraph::AutoParamGraph(te::AutomatableParameter::Ptr ap) : autoParam(ap)
{
    jassert(autoParam != nullptr);
    autoParam->addListener(this);
}

AutoParamGraph::~AutoParamGraph()
{
    autoParam->removeListener(this);
}

void AutoParamGraph::paint(juce::Graphics &g)
{
    points.clear();

    // fill background to show we are automating
    g.setColour(juce::Colours::yellow);
    g.setOpacity(0.25f);
    g.fillRect(getLocalBounds());
    g.setOpacity(1.0f);

    // draw the param name details
    g.setColour(juce::Colours::grey);
    g.setFont(12.0f);
    juce::String textToWrite =
        "Automating " + autoParam->getPluginAndParamName();
    g.drawText(textToWrite, 10, 0, 300, 30, juce::Justification::centredLeft);

    auto curve = autoParam->getCurve();
    auto numPoints = curve.getNumPoints();

    if(numPoints == 0) {
        return;
    }

    for(int i = 0; i < curve.getNumPoints(); i++) {
        auto point = curve.getPoint(i);
        auto displayPoint = convertAutomationPointToDisplayPoint(point);
        juce::Rectangle<float> displayPointArea(6.0, 6.0);
        displayPointArea.setCentre(displayPoint);
        points.emplace_back(PointData {point, displayPoint, displayPointArea});
    }

    g.setColour(juce::Colours::yellow);
    juce::Path path;
    for(auto point = points.begin(); point != points.end(); ++point) {
        // draw lines between automation points
        if(point == points.begin()) {
            path.startNewSubPath(point->displayPoint);
        } else {
            path.lineTo(point->displayPoint);
        }

        // draw point on graph for automation point
        g.fillEllipse(point->displayPointArea);
    }
    g.strokePath(path, juce::PathStrokeType(1.0f));
}

void AutoParamGraph::mouseDown(const juce::MouseEvent &event)
{
    // we're only interested in mouse events with command key down
    if(!event.mods.isCommandDown()) {
        return;
    }

    auto curve = autoParam->getCurve();

    int selectedPointIndex = getPointIndexFromMouseEvent(event);

    if(selectedPointIndex >= 0) {
        if(event.mods.isShiftDown()) {
            curve.removePoint(selectedPointIndex);
        } else {
            indexOfMovingPoint = std::make_unique<int>(selectedPointIndex);
        }
    } else {
        auto tv = convertDisplayPointToTimeAndValue(event.position);

        curve.addPoint(tv.time, tv.value, 0.0f);

        // if this if the first point to be added, create start and end points
        // as well
        if(curve.getNumPoints() == 1) {
            curve.addPoint(0.0, tv.value, 0.0f);
            curve.addPoint(autoParam->getEdit().getLength(), tv.value, 0.0f);
        }
    }
}

void AutoParamGraph::mouseDrag(const juce::MouseEvent &event)
{
    if(indexOfMovingPoint) {
        auto tv = convertDisplayPointToTimeAndValue(event.position);
        auto newIndex = autoParam->getCurve().movePoint(*indexOfMovingPoint,
                                                        tv.time,
                                                        tv.value,
                                                        false);
        indexOfMovingPoint = std::make_unique<int>(newIndex);
    }

    handleTooltip(event);
}

void AutoParamGraph::mouseUp(const juce::MouseEvent &event)
{
    indexOfMovingPoint = nullptr;
}

void AutoParamGraph::mouseMove(const juce::MouseEvent &event)
{
    handleTooltip(event);
}

bool AutoParamGraph::hitTest(int x, int y)
{
    // we're only interested in events where command key is down. Let all others
    // through
    return juce::ModifierKeys::currentModifiers.isCommandDown();
}

// Private methods
void AutoParamGraph::curveHasChanged(te::AutomatableParameter &)
{
    repaint();
}

juce::Point<float> AutoParamGraph::convertAutomationPointToDisplayPoint(
    const te::AutomationCurve::AutomationPoint &point)
{
    auto normalisedTime = point.time / autoParam->getEdit().getLength();
    auto vr = autoParam->getValueRange();
    auto normalisedValue =
        (point.value - vr.getStart()) / (vr.getEnd() - vr.getStart());
    return {getWidth() * float(normalisedTime), getHeight() * normalisedValue};
}

AutoParamGraph::TimeAndValue AutoParamGraph::convertDisplayPointToTimeAndValue(
    const juce::Point<float> &point)
{
    auto vr = autoParam->getValueRange();

    double time = (point.x / getWidth()) * autoParam->getEdit().getLength();
    auto value = ((point.y / getHeight()) * (vr.getEnd() - vr.getStart())) +
                 vr.getStart();

    return {time, value};
}

int AutoParamGraph::getPointIndexFromMouseEvent(const juce::MouseEvent &e)
{
    // do the event coords correspond to a point?
    auto result =
        std::find_if(points.begin(),
                     points.end(),
                     [&e](const PointData &point) {
                         return point.displayPointArea.contains(e.position);
                     });
    return result != points.end() ? result - points.begin() : -1;
}

void AutoParamGraph::handleTooltip(const juce::MouseEvent &e)
{
    // Implementation based on:
    // https://github.com/juce-framework/JUCE/blob/master/examples/GUI/WidgetsDemo.h
    auto it = std::find_if(points.begin(), points.end(), [&e](PointData &p) {
        return p.displayPointArea.contains(e.getPosition().toFloat());
    });

    if(it != points.end()) {
        auto point = *it;
        juce::AttributedString text(juce::String(point.point.value) + " at " +
                                    juce::String(point.point.time));
        text.setColour(juce::Colours::white);
        tooltip.reset(new juce::BubbleMessageComponent());
        addChildComponent(tooltip.get());
        tooltip->showAt(point.displayPointArea.toNearestInt(), text, 500);
    }
}
