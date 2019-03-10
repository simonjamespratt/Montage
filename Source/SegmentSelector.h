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
struct audioSegment
{
  double start;
  double end;
};

class SegmentSelector : public Component, public ChangeBroadcaster
{
public:
  SegmentSelector(AudioTransportSource &transportSourceToUse);
  ~SegmentSelector();

  void paint(Graphics &) override;

  void mouseDown(const MouseEvent &event) override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;
  audioSegment getAudioSegment();

private:
  AudioTransportSource &transportSource;
  int mousePositionA;
  int mousePositionB;
  int selectionStart;
  int selectionEnd;
  audioSegment selectedAudioSegment;

  void handleMouseMovement();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SegmentSelector)
};
