/*
  ==============================================================================

    TransportInteractor.h
    Created: 13 Apr 2019 2:47:28pm
    Author:  Simon Pratt

  ==============================================================================
*/

// Combine the working of cursor and segment selector mouse movement handling
// Default state should be that mouse movement results in cursor movement only - (Cursor logic)
// When put into range selection mode, mouse movement is as in segment selector logic

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/

struct SelectionRange
{
  double rangeStart;
  double rangeEnd;
};

class TransportInteractor : public Component
{
public:
  TransportInteractor(te::TransportControl &tc, te::Edit &e);
  ~TransportInteractor();

  enum InteractionState
  {
    ControlCursor,
    ControlRangeSelection
  };

  void changeMode(InteractionState newState);

  void paint(Graphics &) override;
  void mouseDown(const MouseEvent &event) override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;

  SelectionRange getSelectionRange();

private:
  te::TransportControl &transport;
  te::Edit &edit;
  InteractionState interactionState;
  float mousePositionA;
  float mousePositionB;
  double rangeStart;
  double rangeEnd;

  double calculateAudioPosition(float mousePosition);
  void handleMouseMovement();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportInteractor)
};
