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
        repaint();
        mouseDrag(event);
    }

    if(interactionState == ControlRangeSelection) {
        transport.stop(false, false);
        rangeStart = calculateAudioPosition(event.position.x);
        transport.position = rangeStart;
    }
}

void TransportInteractor::mouseDrag(const juce::MouseEvent &event)
{
    if(interactionState == ControlCursor) {
        transport.position = calculateAudioPosition(event.position.x);
    }

    if(interactionState == ControlRangeSelection) {
        auto mousePositionA = event.getMouseDownPosition().x;
        auto mousePositionB = event.position.x;

        handleMouseMovement(mousePositionA, mousePositionB);
    }
}

void TransportInteractor::mouseUp(const juce::MouseEvent &event)
{
    transport.setUserDragging(false);

    if(interactionState == ControlRangeSelection) {
        auto mousePositionA = event.getMouseDownPosition().x;
        auto mousePositionB = event.position.x;

        handleMouseMovement(mousePositionA, mousePositionB);

        transport.position = rangeStart;
        transport.setLoopRange(te::EditTimeRange(rangeStart, rangeEnd));
        transport.looping = true;
        transport.play(false);

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
    transport.position = rangeStart;
    transport.setLoopRange(te::EditTimeRange(rangeStart, rangeEnd));
    transport.looping = true;
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

void TransportInteractor::handleMouseMovement(int mousePositionA,
                                              int mousePositionB)
{
    if(mousePositionB > mousePositionA && mousePositionB > getWidth()) {
        mousePositionB = getWidth();
    } else if(mousePositionB < mousePositionA && mousePositionB < 0) {
        mousePositionB = 0;
    }

    if(mousePositionB >= mousePositionA) {
        // set the audio segment params
        rangeStart = calculateAudioPosition(mousePositionA);
        rangeEnd = calculateAudioPosition(mousePositionB);
    } else {
        // set the audio segment params
        rangeStart = calculateAudioPosition(mousePositionB);
        rangeEnd = calculateAudioPosition(mousePositionA);
    }

    repaint();
}
