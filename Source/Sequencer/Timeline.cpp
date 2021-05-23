#include "Timeline.h"

Scale::Scale(float i, juce::Rectangle<int> a) : increment(i), area(a)
{}

void Scale::drawScale(double editLength, juce::Graphics &g)
{
    float timePoint = 0.0f;

    while(timePoint < editLength) {
        float x = area.getWidth() / editLength * timePoint;
        performLayout(timePoint, g, x);
        // handles precision errors in incrementing floating points
        timePoint = std::round((timePoint + increment) * 10) / 10;
    }
}

juce::Rectangle<float> Scale::getLineMarker(float x, LineSize size)
{
    float y;
    float lineThickness = 0.5;
    float lineHeight;
    int componentHeight = area.getHeight();

    switch(size) {
    case LineSize::big:
        y = 0.0;
        lineThickness = 1.0;
        lineHeight = componentHeight;
        break;

    case LineSize::medium:
        y = componentHeight / 2;
        lineHeight = componentHeight / 2;
        break;

    case LineSize::small:
        y = componentHeight - (componentHeight / 4);
        lineHeight = componentHeight / 4;
        break;

    default:
        break;
    }

    return juce::Rectangle<float>(x, y, lineThickness, lineHeight);
}

void Scale::drawNumber(float timePoint, float x, juce::Graphics &g)
{
    g.drawText(juce::String(timePoint),
               (x + 2),
               2,
               200,
               area.getHeight(),
               juce::Justification::topLeft);
}

GranularScale::GranularScale(juce::Rectangle<int> a) : Scale(0.1f, a)
{}
void GranularScale::performLayout(float timePoint, juce::Graphics &g, float x)
{
    auto startOfSecond = floor(timePoint);

    if(timePoint == startOfSecond) {
        g.fillRect(getLineMarker(x, LineSize::big));
        drawNumber(timePoint, x, g);
        return;
    }

    if(timePoint == startOfSecond + 0.5f) {
        g.fillRect(getLineMarker(x, LineSize::medium));
        return;
    }

    g.fillRect(getLineMarker(x, LineSize::small));
}

MidScale::MidScale(juce::Rectangle<int> a) : Scale(0.5f, a)
{}
void MidScale::performLayout(float timePoint, juce::Graphics &g, float x)
{
    if(timePoint == floor(timePoint)) {
        if((int)timePoint % 5 == 0) {
            g.fillRect(getLineMarker(x, LineSize::big));
            drawNumber(timePoint, x, g);
            return;
        }
        g.fillRect(getLineMarker(x, LineSize::medium));
        return;
    }

    g.fillRect(getLineMarker(x, LineSize::small));
}

CoarseScale::CoarseScale(juce::Rectangle<int> a) : Scale(1.0f, a)
{}
void CoarseScale::performLayout(float timePoint, juce::Graphics &g, float x)
{
    if((int)timePoint % 10 == 0) {
        g.fillRect(getLineMarker(x, LineSize::big));
        drawNumber(timePoint, x, g);
        return;
    }

    if((int)timePoint % 5 == 0) {
        g.fillRect(getLineMarker(x, LineSize::medium));
        return;
    }

    g.fillRect(getLineMarker(x, LineSize::small));
}

// ======================================================================

Timeline::Timeline(te::Edit &e) : edit(e)
{}

void Timeline::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId)); // clear the background
    g.setColour(juce::Colours::grey);
    g.fillRect(getLocalBounds());
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

    auto editLength = edit.getLength();
    float pixelsPerSecond = getWidth() / editLength;

    if(pixelsPerSecond <= 25) {
        scale = std::make_unique<CoarseScale>(getLocalBounds());
    } else if(pixelsPerSecond > 25.0f && pixelsPerSecond <= 50.0f) {
        scale = std::make_unique<MidScale>(getLocalBounds());
    } else {
        scale = std::make_unique<GranularScale>(getLocalBounds());
    }

    if(scale != nullptr) {
        scale->drawScale(editLength, g);
    }
}

void Timeline::recalculate()
{
    repaint();
}
