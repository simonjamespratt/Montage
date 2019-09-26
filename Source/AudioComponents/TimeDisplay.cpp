/*
  ==============================================================================

    TimeDisplay.cpp
    Created: 12 Apr 2019 7:13:35pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TimeDisplay.h"

//==============================================================================
TimeDisplay::TimeDisplay(te::TransportControl &tc) : transport(tc)
{
    startTimer(20);
    addAndMakeVisible(&transportPosition);
}

TimeDisplay::~TimeDisplay()
{
    stopTimer();
}

void TimeDisplay::resized()
{
    transportPosition.setBounds(0, 0, getWidth(), getHeight());
}

void TimeDisplay::timerCallback()
{
    RelativeTime position(transport.getCurrentPosition());
    auto minutes = ((int)position.inMinutes()) % 60;
    auto seconds = ((int)position.inSeconds()) % 60;
    auto millis = ((int)position.inMilliseconds()) % 1000;
    auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    transportPosition.setFont(Font(24.0f));
    transportPosition.setText(positionString, dontSendNotification);
}
