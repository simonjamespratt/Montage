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
  void setFile(const tracktion_engine::AudioFile &file, const double &offset, const double &clipLength);

private:
  tracktion_engine::TransportControl &transport;
  tracktion_engine::SmartThumbnail smartThumbnail;

  double timeRangeStart;
  double timeRangeEnd;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TracktionThumbnail)
};
