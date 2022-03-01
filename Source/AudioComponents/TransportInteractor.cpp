#include "TransportInteractor.h"

#include "Helpers.h"

TransportInteractor::TransportInteractor(te::Edit &e)
: edit(e), transport(edit.getTransport())
{
    edit.state.addListener(this);
    interactionState = ControlCursor;
}

TransportInteractor::~TransportInteractor()
{
    edit.state.removeListener(this);
}

void TransportInteractor::mouseDown(const juce::MouseEvent &event)
{
    interactionState =
        event.mods.isShiftDown() ? ControlRangeSelection : ControlCursor;

    transport.setUserDragging(true);

    if(interactionState == ControlCursor) {
        auto wasPlaying = transport.isPlaying();

        if(onSelectionChanged) {
            onSelectionChanged();
        }

        transport.looping = false;
        transport.stop(false, false);
        transport.setLoopRange({});

        if(wasPlaying) {
            transport.play(false);
        }

        mouseDrag(event);
    }

    if(interactionState == ControlRangeSelection) {
        transport.stop(false, false);
        mouseDownPosition = event.getPosition().getX();
        auto rangeStart = xToTime(mouseDownPosition);
        transport.setLoopRange({rangeStart, rangeStart});
        transport.setCurrentPosition(rangeStart);
    }
}

void TransportInteractor::mouseDrag(const juce::MouseEvent &event)
{
    if(interactionState == ControlCursor) {
        transport.position = xToTime(event.position.x);
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

        auto loopRange = transport.getLoopRange();
        transport.setCurrentPosition(loopRange.getStart());

        if(onSelectionChanged) {
            onSelectionChanged();
        }
    }
}

SelectionRange TransportInteractor::getSelectionRange()
{
    auto loopRange = transport.getLoopRange();
    return {loopRange.getStart(), loopRange.getEnd()};
}

void TransportInteractor::setSelectionRange(SelectionRange newRange)
{
    transport.setCurrentPosition(newRange.start);
    transport.setLoopRange({newRange.start, newRange.end});
}

void TransportInteractor::clearSelectionRange()
{
    transport.setLoopRange({});

    if(onSelectionChanged) {
        onSelectionChanged();
    }
}

// Private methods
void TransportInteractor::valueTreePropertyChanged(juce::ValueTree &,
                                                   const juce::Identifier &prop)
{
    if(prop == te::IDs::loopPoint1 || prop == te::IDs::loopPoint2) {
        triggerAsyncUpdate();
    }
}

void TransportInteractor::handleAsyncUpdate()
{
    if(onSelectionChanged) {
        onSelectionChanged();
    }
}

double TransportInteractor::xToTime(float mousePosition)
{
    jassert(getWidth() > 0);
    return Helpers::xToTime(mousePosition, edit.getLength(), getWidth());
}

void TransportInteractor::handleMouseMovement(int mousePosition)
{
    if(mousePosition > mouseDownPosition && mousePosition > getWidth()) {
        mousePosition = getWidth();
    } else if(mousePosition < mouseDownPosition && mousePosition < 0) {
        mousePosition = 0;
    }

    double rangeStart, rangeEnd;

    if(mousePosition >= mouseDownPosition) {
        // set the audio segment params
        rangeStart = xToTime(mouseDownPosition);
        rangeEnd = xToTime(mousePosition);
    } else {
        // set the audio segment params
        rangeStart = xToTime(mousePosition);
        rangeEnd = xToTime(mouseDownPosition);
    }
    transport.setLoopRange({rangeStart, rangeEnd});
}
