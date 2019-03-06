/*
  ==============================================================================

    PositionMarker.cpp
    Created: 4 Mar 2019 5:55:03pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PositionMarker.h"

//==============================================================================
PositionMarker::PositionMarker(AudioTransportSource &transportSourceToUse) : transportSource(transportSourceToUse)
{
    startTimer(40);
}

PositionMarker::~PositionMarker()
{
    stopTimer();
}

void PositionMarker::paint(Graphics &g)
{
    auto audioLength = transportSource.getLengthInSeconds();
    if (audioLength > 0.0)
    {
        auto audioPosition = transportSource.getCurrentPosition();
        auto xaxisPosition = (audioPosition / audioLength) * getWidth();
        g.setColour(Colours::green);
        g.drawLine(xaxisPosition, 0.0f, xaxisPosition, (float)getHeight(), 2.0f);
    }
}

// void PositionMarker::mouseDown(const MouseEvent &event)
// {
// do something
// }

void PositionMarker::timerCallback()
{
    repaint();
};
