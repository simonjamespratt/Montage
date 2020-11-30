/*
  ==============================================================================

    TimeDisplay.h
    Created: 12 Apr 2019 7:13:35pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once
#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

//==============================================================================
/*
 */
class TimeDisplay : public juce::Component, public juce::Timer {
  public:
    TimeDisplay(te::TransportControl &tc);
    ~TimeDisplay();

    void resized() override;

    void timerCallback() override;

  private:
    te::TransportControl &transport;
    juce::Label transportPosition;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeDisplay)
};
