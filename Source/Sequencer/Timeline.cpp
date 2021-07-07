#include "Timeline.h"

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
    float timePoint = 0.0;
    float increment = 0.1;

    while(timePoint < editLength) {
        float x = getWidth() / editLength * timePoint;
        float base = floor(timePoint);
        float midway = base + 0.5;

        if(timePoint == base) {
            g.fillRect(getLineMarker(x, LineSize::big));

            g.drawText(juce::String(timePoint),
                       (x + 2),
                       2,
                       200,
                       getHeight(),
                       juce::Justification::topLeft);
        } else if(timePoint == midway) {
            g.fillRect(getLineMarker(x, LineSize::medium));
        } else {
            g.fillRect(getLineMarker(x, LineSize::small));
        }

        // handles precision errors in incrementing floating points
        timePoint = std::round((timePoint + increment) * 10) / 10;
    }
}

void Timeline::recalculate()
{
    repaint();
}

// Private methods
juce::Rectangle<float> Timeline::getLineMarker(float x, LineSize size)
{
    float y;
    float lineThickness = 0.5;
    float lineHeight;

    switch(size) {
    case LineSize::big:
        y = 0.0;
        lineThickness = 1.0;
        lineHeight = getHeight();
        break;

    case LineSize::medium:
        y = getHeight() / 2;
        lineHeight = getHeight() / 2;
        break;

    case LineSize::small:
        y = getHeight() - (getHeight() / 4);
        lineHeight = getHeight() / 4;
        break;

    default:
        break;
    }

    return juce::Rectangle<float>(x, y, lineThickness, lineHeight);
}
