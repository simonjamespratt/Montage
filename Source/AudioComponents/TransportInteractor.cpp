#include "TransportInteractor.h"

TransportInteractor::TransportInteractor(te::TransportControl &tc, te::Edit &e)
: transport(tc), edit(e)
{
    interactionState = ControlCursor;
    rangeStart = 0;
    rangeEnd = 0;
}

TransportInteractor::~TransportInteractor()
{}

void TransportInteractor::paint(juce::Graphics &g)
{
    auto mousePositionA = calculateUIPosition(rangeStart);
    auto mousePositionB = calculateUIPosition(rangeEnd);

    g.setColour(juce::Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
    g.fillRect(mousePositionA,
               0.0f,
               (mousePositionB - mousePositionA),
               float(getHeight()));
}

void TransportInteractor::mouseDown(const juce::MouseEvent &event)
{
    if(event.mods.isShiftDown()) {
        interactionState = ControlRangeSelection;
    } else {
        interactionState = ControlCursor;
    }

    transport.setUserDragging(true);

    if(interactionState == ControlCursor) {
        auto wasPlaying = transport.isPlaying();

        rangeStart = 0;
        rangeEnd = 0;
        if(onSelectionChange) {
            onSelectionChange();
        }

        transport.looping = false;
        transport.stop(false, false);
        transport.setLoopRange({});

        if(wasPlaying) {
            transport.play(false);
        }

        repaint();
        mouseDrag(event);
    }

    if(interactionState == ControlRangeSelection) {
        transport.stop(false, false);
        mouseDownPosition = event.getPosition().getX();
        rangeStart = calculateAudioPosition(mouseDownPosition);
        transport.setCurrentPosition(rangeStart);
    }
}

void TransportInteractor::mouseDrag(const juce::MouseEvent &event)
{
    if(interactionState == ControlCursor) {
        transport.position = calculateAudioPosition(event.position.x);
    }

    if(interactionState == ControlRangeSelection) {
        handleMouseMovement(event.getPosition().getX());

        if(onSelectionChangeInProgress) {
            onSelectionChangeInProgress(event);
        }
    }
}

void TransportInteractor::mouseUp(const juce::MouseEvent &event)
{
    transport.setUserDragging(false);

    if(interactionState == ControlRangeSelection) {
        handleMouseMovement(event.getPosition().getX());

        transport.setCurrentPosition(rangeStart);
        transport.setLoopRange({rangeStart, rangeEnd});

        if(onSelectionChange) {
            onSelectionChange();
        }
    }
}

SelectionRange TransportInteractor::getSelectionRange()
{
    return {rangeStart, rangeEnd};
}

void TransportInteractor::setSelectionRange(SelectionRange newRange)
{
    rangeStart = newRange.rangeStart;
    rangeEnd = newRange.rangeEnd;
    transport.setCurrentPosition(rangeStart);
    transport.setLoopRange({rangeStart, rangeEnd});
    repaint();
}

void TransportInteractor::clearSelectionRange()
{
    rangeStart = 0;
    rangeEnd = 0;

    if(onSelectionChange) {
        onSelectionChange();
    }
}

// Private methods
double TransportInteractor::calculateAudioPosition(float mousePosition)
{
    jassert(getWidth() > 0);
    const double proportion = mousePosition / getWidth();
    return proportion * edit.getLength();
}

float TransportInteractor::calculateUIPosition(double rangePosition)
{
    jassert(getWidth() > 0);
    auto editLength = edit.getLength();
    auto proportion = editLength ? rangePosition / editLength : 0;
    return getWidth() * proportion;
}

void TransportInteractor::handleMouseMovement(int mousePosition)
{
    if(mousePosition > mouseDownPosition && mousePosition > getWidth()) {
        mousePosition = getWidth();
    } else if(mousePosition < mouseDownPosition && mousePosition < 0) {
        mousePosition = 0;
    }

    if(mousePosition >= mouseDownPosition) {
        // set the audio segment params
        rangeStart = calculateAudioPosition(mouseDownPosition);
        rangeEnd = calculateAudioPosition(mousePosition);
    } else {
        // set the audio segment params
        rangeStart = calculateAudioPosition(mousePosition);
        rangeEnd = calculateAudioPosition(mouseDownPosition);
    }

    repaint();
}
