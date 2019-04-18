/*
  ==============================================================================

    TransportInteractor.cpp
    Created: 13 Apr 2019 2:47:28pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TransportInteractor.h"

//==============================================================================
TransportInteractor::TransportInteractor(te::TransportControl &tc, te::Edit &e) : transport(tc), edit(e)
{
    interactionState = ControlCursor;
    rangeStart = 0.0;
    rangeEnd = 0.0;
}

TransportInteractor::~TransportInteractor()
{
}

SelectionRange TransportInteractor::getSelectionRange()
{
    return {rangeStart, rangeEnd};
}

void TransportInteractor::paint(Graphics &g)
{
    if (interactionState == ControlRangeSelection)
    {
        g.setColour(Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
        g.fillRect(mousePositionA, 0.0f, (mousePositionB - mousePositionA), float(getHeight()));
    }
}

void TransportInteractor::mouseDown(const MouseEvent &event)
{
    if (event.mods.isShiftDown())
    {
        interactionState = ControlRangeSelection;
    }
    else
    {
        interactionState = ControlCursor;
    }

    transport.setUserDragging(true);

    if (interactionState == ControlCursor)
    {
        repaint();
        rangeStart = 0.0;
        rangeEnd = 0.0;
        transport.setLoopRange(te::EditTimeRange(0.0, edit.getLength()));
        mouseDrag(event);
    }

    if (interactionState == ControlRangeSelection)
    {
        transport.stop(false, false);
        mousePositionA = event.position.x;
        rangeStart = calculateAudioPosition(mousePositionA);
        transport.position = rangeStart;
    }
}

void TransportInteractor::mouseDrag(const MouseEvent &event)
{
    if (interactionState == ControlCursor)
    {
        transport.position = calculateAudioPosition(event.position.x);
    }

    if (interactionState == ControlRangeSelection)
    {
        mousePositionB = event.position.x;
        handleMouseMovement();
    }
}

void TransportInteractor::mouseUp(const MouseEvent &event)
{
    transport.setUserDragging(false);

    if (interactionState == ControlRangeSelection)
    {
        handleMouseMovement();
        transport.position = rangeStart;
        transport.setLoopRange(te::EditTimeRange(rangeStart, rangeEnd));
        transport.looping = true;
        transport.play(false);
    }
}

double TransportInteractor::calculateAudioPosition(float mousePosition)
{
    jassert(getWidth() > 0);
    const double proportion = mousePosition / getWidth();
    return proportion * edit.getLength();
}

void TransportInteractor::handleMouseMovement()
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
        rangeStart = calculateAudioPosition(mousePositionA);
        rangeEnd = calculateAudioPosition(mousePositionB);
    }
    else
    {
        // set the audio segment params
        rangeStart = calculateAudioPosition(mousePositionB);
        rangeEnd = calculateAudioPosition(mousePositionA);
    }

    repaint();
}
