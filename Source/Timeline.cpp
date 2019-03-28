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

    // TODO: draw a marker every 500ms
    // TODO: draw text for each marker

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText(String(editLength), getLocalBounds(),
               Justification::centred, true); // draw some placeholder text
}

void Timeline::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void Timeline::recalculate()
{
    editLength = edit.getLength();
    repaint();
}
