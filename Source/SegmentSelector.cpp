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
    selectedAudioSegment.state = NoSelection;
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
    // TODO: reinstate this when we have a sense of state for being in selecting mode or not

    // auto audioLength = transportSource.getLengthInSeconds();
    // if (audioLength > 0.0)
    // {
    //     auto clickPosition = event.position.x;
    //     auto audioPosition = (clickPosition / getWidth()) * audioLength;
    //     transportSource.setPosition(audioPosition);
    // }

    mousePositionA = event.x;

    selectedAudioSegment.state = SelectionStarted;
    selectedAudioSegment.start = calculateAudioPosition(mousePositionA);
    // trigger the change broadcaster
    sendChangeMessage();
}

void SegmentSelector::mouseDrag(const MouseEvent &event)
{
    mousePositionB = event.x;
    handleMouseMovement();
}

void SegmentSelector::mouseUp(const MouseEvent &event)
{
    mousePositionB = event.x;
    selectedAudioSegment.state = SelectionComplete;
    handleMouseMovement();
}

double SegmentSelector::calculateAudioPosition(int mousePosition)
{
    auto audioLength = transportSource.getLengthInSeconds();
    return (audioLength / getWidth()) * mousePosition;
};

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
        // set the audio segment params
        selectedAudioSegment.start = calculateAudioPosition(mousePositionA);
        selectedAudioSegment.end = calculateAudioPosition(mousePositionB);
    }
    else
    {
        // set the audio segment params
        selectedAudioSegment.start = calculateAudioPosition(mousePositionB);
        selectedAudioSegment.end = calculateAudioPosition(mousePositionA);
    }

    // trigger the change broadcaster
    sendChangeMessage();

    repaint();
}

AudioSegment SegmentSelector::getAudioSegment()
{
    return selectedAudioSegment;
}
