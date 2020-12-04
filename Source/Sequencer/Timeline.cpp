#include "Timeline.h"

Timeline::Timeline(te::Edit &e) : edit(e)
{
    editLength = edit.getLength();
}

Timeline::~Timeline()
{}

void Timeline::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(juce::Colours::grey);
    g.fillRect(getLocalBounds());

    double timePoint = 0.0;
    while(timePoint < editLength) {
        auto x = getWidth() / editLength * timePoint;
        g.setColour(juce::Colours::white);
        g.drawLine(x, 0.0, x, getHeight(), 2);
        g.drawText(juce::String(timePoint),
                   (x + 2),
                   0,
                   200,
                   30,
                   juce::Justification::centredLeft);
        timePoint = timePoint + 0.5;
    }
}

void Timeline::recalculate()
{
    editLength = edit.getLength();
    repaint();
}
