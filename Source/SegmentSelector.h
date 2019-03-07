/*
  ==============================================================================

    SegmentSelector.h
    Created: 6 Mar 2019 7:35:32pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SegmentSelector : public Component
{
  public:
    SegmentSelector(AudioTransportSource &transportSourceToUse);
    ~SegmentSelector();

    void paint(Graphics &) override;

    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;

  private:
    AudioTransportSource &transportSource;
    int selectionStart;
    int selectionEnd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SegmentSelector)
};
