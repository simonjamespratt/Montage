#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : state(Stopped),
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

    startTimer(20);

    // Set the size of the component after you add any child components
    setSize(600, 400);

    // Initialise audio handling
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    segmentSelector.addChangeListener(this);
    audioSegment = segmentSelector.getAudioSegment();

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    stopTimer();
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
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

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

// state and event handlers
void MainComponent::changeState(TransportState newState)
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
void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
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

void MainComponent::segmentSelectorChanged()
{
    audioSegment = segmentSelector.getAudioSegment();
    if (audioSegment.start != 0.0 && audioSegment.end != 0.0)
    {
        transportSource.setPosition(audioSegment.start);

        if (state != Playing || state != Starting)
        {
            changeState(Starting);
        }
    }
}

// this is the callback that is used when the transportSource change listener event occurs
void MainComponent::transportSourceChanged()
{
    if (transportSource.isPlaying())
        changeState(Playing);
    else if ((state == Stopping) || (state == Playing))
        changeState(Stopped);
    else if (Pausing == state)
        changeState(Paused);
}

void MainComponent::openButtonClicked()
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

void MainComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
}

void MainComponent::stopButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void MainComponent::timerCallback()
{
    RelativeTime position(transportSource.getCurrentPosition());
    auto minutes = ((int)position.inMinutes()) % 60;
    auto seconds = ((int)position.inSeconds()) % 60;
    auto millis = ((int)position.inMilliseconds()) % 1000;
    auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    currentTimePosition.setText(positionString, dontSendNotification);

    if (transportSource.isPlaying() && audioSegment.start != 0.0 && audioSegment.end != 0.0)
    {
        if (transportSource.getCurrentPosition() >= audioSegment.end)
        {
            changeState(Pausing);
        }
    }
}

//==============================================================================
void MainComponent::paint(Graphics &g)
{
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    currentTimePosition.setBounds(10, 100, getWidth() - 20, 20);
    Rectangle<int> thumbnailBounds(10, 130, getWidth() - 20, getHeight() - 150);
    waveformDisplay.setBounds(thumbnailBounds);
    positionMarker.setBounds(thumbnailBounds);
    segmentSelector.setBounds(thumbnailBounds);
}
