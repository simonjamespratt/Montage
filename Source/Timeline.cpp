/*
  ==============================================================================

    Timeline.cpp
    Created: 27 Mar 2019 7:59:55pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Timeline.h"

//==============================================================================
Timeline::Timeline(te::Edit &e) : edit(e)
{
    editLength = edit.getLength();
}

Timeline::~Timeline()
{
}

void Timeline::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(Colours::grey);
    g.fillRect(getLocalBounds());

    double timePoint = 0.0;
    while (timePoint < editLength)
    {
        auto x = getWidth() / editLength * timePoint;
        g.setColour(Colours::white);
        g.drawLine(x, 0.0, x, getHeight(), 2);
        g.drawText(String(timePoint), (x + 2), 0, 200, 30, Justification::centredLeft);
        timePoint = timePoint + 0.5;
    }
}

void Timeline::recalculate()
{
    editLength = edit.getLength();
    repaint();
}
