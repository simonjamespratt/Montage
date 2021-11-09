#include "AudioEditViewport.h"

void AudioEditViewport::visibleAreaChanged(
    const juce::Rectangle<int> &newVisibleArea)
{
    if(onVisibleAreaChanged) {
        onVisibleAreaChanged(newVisibleArea);
    }
}

void AudioEditViewport::syncToTransportPositionWhenPlaying(
    double transportPosition, double editLength)
{
    auto transportPositionAsPointInViewedComponent =
        getTransportPositionAsPointInViewedComponent(transportPosition,
                                                     editLength);

    auto viewArea = getViewArea();

    if(transportPositionAsPointInViewedComponent >= 0 &&
       !viewArea.getHorizontalRange().contains(
           transportPositionAsPointInViewedComponent)) {
        auto existingYOffset = viewArea.getY();
        auto newXOffset = transportPositionAsPointInViewedComponent;

        setViewPosition(newXOffset, existingYOffset);
    }
}

void AudioEditViewport::syncToTransportPositionOnResize(
    double transportPosition, double editLength)
{
    // set the viewed component position within the viewport to be centered
    // around the current transport position
    auto centreOfViewport = getWidth() * 0.5;
    auto arrangementXOffset =
        getTransportPositionAsPointInViewedComponent(transportPosition,
                                                     editLength) -
        centreOfViewport;
    auto existingYOffset = getViewArea().getY();

    setViewPosition(arrangementXOffset, existingYOffset);
}

void AudioEditViewport::syncToMouseDrag(const juce::MouseEvent event)
{
    auto mousePosition = event.getPosition().getX();
    auto viewArea = getViewArea();
    auto horizontalRangeOfViewArea = viewArea.getHorizontalRange();
    auto rangeStart = horizontalRangeOfViewArea.getStart();
    auto rangeEnd = horizontalRangeOfViewArea.getEnd();
    int tolerance = 20;

    if(mousePosition > (rangeEnd - tolerance)) {
        auto nudgeValue = mousePosition - (rangeEnd - tolerance);
        auto newXOffset = rangeStart + nudgeValue;

        auto existingYOffset = viewArea.getY();

        setViewPosition(newXOffset, existingYOffset);

        return;
    }

    if(mousePosition < (rangeStart + tolerance)) {
        auto nudgeValue = (rangeStart + tolerance) - mousePosition;
        auto newXOffset = rangeStart - nudgeValue;

        auto existingYOffset = viewArea.getY();

        setViewPosition(newXOffset, existingYOffset);

        return;
    }
}

// Private methods
double AudioEditViewport::getTransportPositionAsPointInViewedComponent(
    double transportPosition, double editLength)
{
    auto normalisedTransportPosition = transportPosition / editLength;
    return normalisedTransportPosition * getViewedComponent()->getWidth();
}
