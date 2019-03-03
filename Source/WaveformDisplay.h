/*
  ==============================================================================

    WaveformDisplay.h
    Created: 28 Feb 2019 8:19:32pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay : public Component, public ChangeListener
{
public:
  WaveformDisplay(int sourceSamplesPerThumbnailSample, AudioFormatManager &formatManager, AudioThumbnailCache &cache);

  void paint(Graphics &) override;
  void changeListenerCallback(ChangeBroadcaster *source) override;
  void setFile(const File &file);

private:
  AudioThumbnail thumbnail;
  void thumbnailChanged();

  // waveform drawing functions
  void paintIfNoFileLoaded(Graphics &g);
  void paintIfFileLoaded(Graphics &g);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
