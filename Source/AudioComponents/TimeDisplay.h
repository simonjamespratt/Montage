/*
  ==============================================================================

    TimeDisplay.h
    Created: 12 Apr 2019 7:13:35pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class TimeDisplay : public Component, public Timer
{
public:
  TimeDisplay(te::TransportControl &tc);
  ~TimeDisplay();

  void resized() override;

  void timerCallback() override;

private:
  te::TransportControl &transport;
  Label transportPosition;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeDisplay)
};
