#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"
#include "PositionMarker.h"
#include "SegmentSelector.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent, public ChangeListener, public Timer
{
public:
  // constructor and deconstructor
  MainComponent();
  ~MainComponent();

  // AudioSource overrides
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  // AudioTransportSource (ChangeBroadaster) override
  void changeListenerCallback(ChangeBroadcaster *source) override;

  void timerCallback() override;

  // GUI Component overrides
  void paint(Graphics &g) override;
  void resized() override;

private:
  enum TransportState
  {
    Starting,
    Playing,
    Pausing,
    Paused,
    Stopping,
    Stopped,
  };
  TransportState state;

  TextButton openButton;
  TextButton playButton;
  TextButton stopButton;
  Label currentTimePosition;

  AudioFormatManager formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;

  AudioThumbnailCache thumbnailCache;
  WaveformDisplay waveformDisplay;
  PositionMarker positionMarker;
  SegmentSelector segmentSelector;
  AudioSegment audioSegment;

  void changeState(TransportState newState);
  void openButtonClicked();
  void playButtonClicked();
  void stopButtonClicked();

  // change listener callbacks
  void transportSourceChanged();
  void segmentSelectorChanged();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
