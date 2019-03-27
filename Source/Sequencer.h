/*
  ==============================================================================

    Sequencer.h
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./TracktionThumbnail.h"

//==============================================================================
/*
*/
class Sequencer : public Component, public ChangeListener, public Timer
{
public:
  Sequencer();
  ~Sequencer();

  void resized() override;
  void timerCallback() override;

private:
  tracktion_engine::Engine engine;
  // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
  // when the sequencer is working seriously, probably need to change this
  tracktion_engine::Edit edit;
  tracktion_engine::TransportControl &transport;

  TextButton playPauseButton;
  TextButton stopButton;
  TextButton settingsButton;
  Label transportPosition;
  TextButton loadFileButton;

  TracktionThumbnail thumbnail;

  void showAudioDeviceSettings(tracktion_engine::Engine &engine);

  void selectAudioFile();
  void setFile(const File &file);

  void changeListenerCallback(ChangeBroadcaster *) override;
  void togglePlayPause();
  void stop();
  void updatePlayPauseButtonText();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
