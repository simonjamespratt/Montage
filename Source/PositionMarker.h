/*
  ==============================================================================

    PositionMarker.h
    Created: 4 Mar 2019 5:55:03pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PositionMarker : public Component, public Timer
{
public:
  PositionMarker(AudioTransportSource &transportSourceToUse);
  ~PositionMarker();

  void paint(Graphics &) override;

private:
  AudioTransportSource &transportSource;
  void timerCallback() override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionMarker)
};
