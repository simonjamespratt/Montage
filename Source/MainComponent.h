#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public ChangeListener
{
public:
    // constructor and deconstructor
    MainComponent();
    ~MainComponent();

    // AudioSource overrides
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // AudioTransportSource (ChangeBroadaster) override
    void changeListenerCallback (ChangeBroadcaster* source) override;

    // GUI Component overrides
    void paint (Graphics& g) override;
    void resized() override;

private:
    enum TransportState {
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

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;

    void changeState (TransportState newState);
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
