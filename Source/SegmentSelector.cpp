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
    selectedAudioSegment.start = 0.0;
    selectedAudioSegment.end = 0.0;
}

SegmentSelector::~SegmentSelector()
{
}

void SegmentSelector::paint(Graphics &g)
{
    g.setColour(Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
    g.fillRect(mousePositionA, 0, (mousePositionB - mousePositionA), getHeight());
}

void SegmentSelector::mouseDown(const MouseEvent &event)
{
    auto audioLength = transportSource.getLengthInSeconds();
    if (audioLength > 0.0)
    {
        auto clickPosition = event.position.x;
        auto audioPosition = (clickPosition / getWidth()) * audioLength;
        transportSource.setPosition(audioPosition);
    }

    mousePositionA = event.x;
}

void SegmentSelector::mouseDrag(const MouseEvent &event)
{
    mousePositionB = event.x;
    handleMouseMovement();
}

void SegmentSelector::mouseUp(const MouseEvent &event)
{
    mousePositionB = event.x;
    handleMouseMovement();
}

void SegmentSelector::handleMouseMovement()
{
    if (mousePositionB > mousePositionA && mousePositionB > getWidth())
    {
        mousePositionB = getWidth();
    }
    else if (mousePositionB < mousePositionA && mousePositionB < 0)
    {
        mousePositionB = 0;
    }

    if (mousePositionB >= mousePositionA)
    {
        selectionStart = mousePositionA;
        selectionEnd = mousePositionB;
    }
    else
    {
        selectionStart = mousePositionB;
        selectionEnd = mousePositionA;
    }

    // set the audio segment params
    auto audioLength = transportSource.getLengthInSeconds();
    selectedAudioSegment.start = (audioLength / getWidth()) * selectionStart;
    selectedAudioSegment.end = (audioLength / getWidth()) * selectionEnd;

    // trigger the change broadcaster
    sendChangeMessage();

    repaint();
}

audioSegment SegmentSelector::getAudioSegment()
{
    return selectedAudioSegment;
}
