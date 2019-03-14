/*
  ==============================================================================

    Source.cpp
    Created: 13 Mar 2019 8:35:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Source.h"

//==============================================================================
Source::Source() : state(Stopped),
                   thumbnailCache(5),
                   waveformDisplay(512, formatManager, thumbnailCache),
                   positionMarker(transportSource),
                   segmentSelector(transportSource)
{
    // Initialise display
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.setColour(TextButton::buttonColourId, Colours::red);
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setEnabled(false);

    addAndMakeVisible(&currentTimePosition);
    addAndMakeVisible(&waveformDisplay);
    addAndMakeVisible(&positionMarker);
    addAndMakeVisible(&segmentSelector);

    // Set the size of the component after you add any child components
    setSize(600, 400);

    // Initialise audio handling
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    segmentSelector.addChangeListener(this);
    audioSegment = segmentSelector.getAudioSegment();
    setAudioChannels(0, 2);

    startTimer(20);
}

Source::~Source()
{
    stopTimer();
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void Source::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Source::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    // Your audio-processing code goes here!
    if (readerSource.get() == nullptr)
    {
        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void Source::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

void Source::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        switch (state)
        {
        case Stopped:
            playButton.setButtonText("Play");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(false);
            transportSource.setPosition(0.0);
            break;
        case Starting:
            transportSource.start();
            break;
        case Playing:
            playButton.setButtonText("Pause");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(true);
            break;
        case Pausing:
            transportSource.stop();
            break;
        case Paused:
            playButton.setButtonText("Resume");
            stopButton.setButtonText("Return to Zero");
            break;
        case Stopping:
            transportSource.stop();
            break;
        }
    }
}

// this is the callback that is used when ALL change listener events occur
void Source::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &transportSource)
    {
        transportSourceChanged();
    }

    if (source == &segmentSelector)
    {
        segmentSelectorChanged();
    }
}

void Source::segmentSelectorChanged()
{
    audioSegment = segmentSelector.getAudioSegment();
    if (audioSegment.state == SelectionStarted)
    {
        changeState(Pausing);
        transportSource.setPosition(audioSegment.start);
    }

    if (audioSegment.state == SelectionComplete)
    {
        transportSource.setPosition(audioSegment.start);
        changeState(Starting);
    }
}

void Source::transportSourceChanged()
{
    if (transportSource.isPlaying())
        changeState(Playing);
    else if ((state == Stopping) || (state == Playing))
        changeState(Stopped);
    else if (Pausing == state)
        changeState(Paused);
}

// Button callbacks
void Source::openButtonClicked()
{
    FileChooser chooser("Select file to play", {}, "*.wav,*.aif, *.aiff");

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto *reader = formatManager.createReaderFor(file);

        if (reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled(true);
            waveformDisplay.setFile(file);
            readerSource.reset(newSource.release());
        }
    }
}

void Source::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
}

void Source::stopButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void Source::timerCallback()
{
    RelativeTime position(transportSource.getCurrentPosition());
    auto minutes = ((int)position.inMinutes()) % 60;
    auto seconds = ((int)position.inSeconds()) % 60;
    auto millis = ((int)position.inMilliseconds()) % 1000;
    auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    currentTimePosition.setText(positionString, dontSendNotification);

    if (transportSource.isPlaying() && audioSegment.state == SelectionComplete)
    {
        if (transportSource.getCurrentPosition() >= audioSegment.end)
        {
            changeState(Pausing);
            transportSource.setPosition(audioSegment.start);
        }
    }
}

//==============================================================================
void Source::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    currentTimePosition.setBounds(10, 100, getWidth() - 20, 20);
    Rectangle<int> thumbnailBounds(10, 130, getWidth() - 20, getHeight() - 150);
    waveformDisplay.setBounds(thumbnailBounds);
    positionMarker.setBounds(thumbnailBounds);
    segmentSelector.setBounds(thumbnailBounds);
}
