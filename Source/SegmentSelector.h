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

// NB: I don't know how to include these type definitions within the class and then make them available within the .cpp file
enum SegmentState
{
  NoSelection,
  SelectionStarted,
  SelectionComplete,
};

struct AudioSegment
{
  SegmentState state;
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
  AudioSegment getAudioSegment();

private:
  AudioTransportSource &transportSource;
  int mousePositionA;
  int mousePositionB;
  AudioSegment selectedAudioSegment;

  double calculateAudioPosition(int mousePosition);
  void handleMouseMovement();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SegmentSelector)
};
