/*
  ==============================================================================

    SegmentSelector.cpp
    Created: 6 Mar 2019 7:35:32pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SegmentSelector.h"

//==============================================================================
SegmentSelector::SegmentSelector(AudioTransportSource &transportSourceToUse) : transportSource(transportSourceToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

SegmentSelector::~SegmentSelector()
{
}

void SegmentSelector::paint(Graphics &g)
{
    g.setColour(Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
    g.fillRect(selectionStart, 0, (selectionEnd - selectionStart), getHeight());
}

void SegmentSelector::mouseDown(const MouseEvent &event)
{
    selectionStart = event.position.x;
    // TODO: make it so that when mouse down, an initial line appears so that the user gets some visual cue that selection is in progress
    selectionEnd = event.position.x + 10;
    repaint();
}

void SegmentSelector::mouseDrag(const MouseEvent &event)
{
    selectionEnd = event.position.x;
    // TODO: handle when the user drags outside of the bounds of this component - either side - set the selection end to the start OR end of the component x-axis
    // TODO: what happens if the user drags left instead of right? Visually, this is handled, but what about transport position?
    repaint();
}

void SegmentSelector::mouseUp(const MouseEvent &event)
{
    selectionEnd = event.position.x;
    // TODO: handle when the user drags outside of the bounds of this component - either side - set the selection end to the start OR end of the component x-axis
    // TODO: what happens if the user drags left instead of right? Visually, this is handled, but what about transport position?
    repaint();
}
