/*
  ==============================================================================

    Source.h
    Created: 13 Mar 2019 8:35:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"
#include "PositionMarker.h"
#include "SegmentSelector.h"

//==============================================================================
/*
*/
class Source : public AudioAppComponent, public ChangeListener, public Timer
{
  public:
    Source();
    ~Source();

    // AudioSource overrides
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

    void timerCallback() override;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Source)
};
