#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : state(Stopped)
{
    // Initialise display
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.setColour (TextButton::buttonColourId, Colours::green);
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setEnabled (false);

    addAndMakeVisible (&stopButton);
    stopButton.setButtonText ("Stop");
    stopButton.setColour (TextButton::buttonColourId, Colours::red);
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setEnabled (false);

    // Set the size of the component after you add any child components
    setSize (300, 200);

    // Initialise audio handling
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);

    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    if (readerSource.get() == nullptr)
    {
        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    transportSource.releaseResources();
}

// state and event handlers
void MainComponent::changeState (TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        switch (state)
        {
            case Stopped:                           // [3]
                stopButton.setEnabled (false);
                playButton.setEnabled (true);
                transportSource.setPosition (0.0);
                break;
            case Starting:                          // [4]
                playButton.setEnabled (false);
                transportSource.start();
                break;
            case Playing:                           // [5]
                stopButton.setEnabled (true);
                break;
            case Stopping:                          // [6]
                transportSource.stop();
                break;
        }
    }
}

// this is the callback that is used when the transportSource change listener event occurs
void MainComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else
            changeState (Stopped);
    }
}

void MainComponent::openButtonClicked()
{
    FileChooser chooser("Select file to play", {}, "*.wav,*.aif, *.aiff");

    if (chooser.browseForFileToOpen()) {
        auto file = chooser.getResult();
        auto* reader = formatManager.createReaderFor(file);

        if (reader != nullptr) {
            std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
            transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled (true);
            readerSource.reset (newSource.release());
        }
    }
}

void MainComponent::playButtonClicked()
{
    changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // NB: MAY NOT NEED THIS IF ALL COMPONENTS ARE ADDED AS CHILD COMPONENTS...
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    openButton.setBounds (10, 10, getWidth() - 20, 20);
    playButton.setBounds (10, 40, getWidth() - 20, 20);
    stopButton.setBounds (10, 70, getWidth() - 20, 20);
}
