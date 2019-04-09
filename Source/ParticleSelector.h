/*
  ==============================================================================

    ParticleSelector.h
    Created: 11 Apr 2019 8:08:44pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./TimeDisplay.h"
#include "./TracktionThumbnail.h"
#include "./Cursor.h"
#include "./TransportInteractor.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
// TODO: delete (using Projucer) any components that are now unused
class ParticleSelector : public Component, public ChangeListener
{
public:
  ParticleSelector(te::Engine &eng);
  ~ParticleSelector();

  void resized() override;

private:
  te::Engine &engine;
  // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
  // when this is working seriously, probably need to change this
  te::Edit edit;
  te::TransportControl &transport;
  TracktionThumbnail thumbnail;
  Cursor cursor;
  TimeDisplay transportPosition;
  TransportInteractor transportInteractor;

  TextButton loadFileButton;
  TextButton playPauseButton;
  TextButton stopButton;

  void selectAudioFile();
  void addFileToEditAndLoop(File &file, te::AudioFile &audioFile);
  void updatePlayPauseButtonText();
  void changeListenerCallback(ChangeBroadcaster *) override;
  void togglePlayPause();
  void stop();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticleSelector)
};
