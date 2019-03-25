/*
  ==============================================================================

    TracktionThumbnail.h
    Created: 25 Mar 2019 9:00:26am
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class TracktionThumbnail : public Component
{
public:
  TracktionThumbnail(tracktion_engine::TransportControl &tc);
  ~TracktionThumbnail();

  void paint(Graphics &) override;

  void setFile(const tracktion_engine::AudioFile &file);
  void mouseDown(const MouseEvent &e) override;
  void mouseDrag(const MouseEvent &e) override;
  void mouseUp(const MouseEvent &) override;

private:
  tracktion_engine::TransportControl &transport;
  tracktion_engine::SmartThumbnail smartThumbnail;
  tracktion_engine::LambdaTimer cursorUpdater;
  DrawableRectangle cursor;

  void updateCursorPosition();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TracktionThumbnail)
};
