/*
  ==============================================================================

    Sequencer.cpp
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

//==============================================================================
Sequencer::Sequencer() : engine(ProjectInfo::projectName),
                         edit(engine, tracktion_engine::createEmptyEdit(), tracktion_engine::Edit::forEditing, nullptr, 0),
                         transport(edit.getTransport()),
                         thumbnail(transport)
{
    addAndMakeVisible(&loadFileButton);
    loadFileButton.setButtonText("Load file");
    loadFileButton.onClick = [this] { selectAudioFile(); };

    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] { showAudioDeviceSettings(engine); };

    startTimer(20);
    transport.addChangeListener(this);

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonText();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&transportPosition);
    addAndMakeVisible(&thumbnail);

    setSize(600, 400);
}

Sequencer::~Sequencer()
{
    stopTimer();
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::resized()
{
    loadFileButton.setBounds(10, 10, getWidth() - 20, 20);
    settingsButton.setBounds(10, 40, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 70, getWidth() - 20, 20);
    stopButton.setBounds(10, 100, getWidth() - 20, 20);
    transportPosition.setBounds(10, 130, getWidth() - 20, 20);
    thumbnail.setBounds(10, 170, getWidth() - 20, 200);
}

void Sequencer::showAudioDeviceSettings(tracktion_engine::Engine &engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    o.content.setOwned(new AudioDeviceSelectorComponent(engine.getDeviceManager().deviceManager,
                                                        0, 512, 1, 512, false, false, true, true));
    o.content->setSize(400, 600);
    o.launchAsync();
}

void Sequencer::selectAudioFile()
{
    auto fileChooser = std::make_shared<FileChooser>(
        "Load an audio file",
        engine.getPropertyStorage().getDefaultLoadSaveDirectory(ProjectInfo::projectName), // TODO: I don't think this does anything - check if it can be removed
        "*.wav,*.aif,*.aiff");

    if (fileChooser->browseForFileToOpen())
    {
        auto file = fileChooser->getResult();

        // NB. Again, I don't think this actually does anything!
        // TODO:  check if this can be removed
        if (file.existsAsFile())
        {
            engine.getPropertyStorage().setDefaultLoadSaveDirectory(
                ProjectInfo::projectName,
                file.getParentDirectory());
        }

        setFile(file);
    }
}

void Sequencer::setFile(const File &file)
{
    // find the first track
    auto trackOne = edit.getOrInsertAudioTrackAt(0);

    if (!trackOne)
    {
        return;
    }

    // remove / delete all clips from it if it has any
    auto clipsToRemove = trackOne->getClips();
    for (int i = clipsToRemove.size(); --i >= 0;)
    {
        clipsToRemove.getUnchecked(i)->removeFromParentTrack();
    }

    // add a new clip to this track
    tracktion_engine::AudioFile audioFile(file);

    if (!audioFile.isValid())
    {
        return;
    }

    auto newClip = trackOne->insertWaveClip(
        file.getFileNameWithoutExtension(),
        file,
        {{0.0, audioFile.getLength()}, 0.0}, // NB. this is a ClipPosition, where (I think): { {startClip, endClip}, offset }
        // TODO: ClipPosition: work out how to select a portion of an audio file as the clip instead of using all of it (i.e. a particle)
        // TODO: ClipPosition: work out how to position a clip on a track at a certain offset from the tranport start
        false);

    if (!newClip)
    {
        return;
    }

    // thumbnail using the clip
    transport.setLoopRange(newClip->getEditTimeRange());
    transport.looping = true;
    transport.position = 0.0;
    transport.play(false);
    thumbnail.setFile(newClip->getPlaybackFile());
}

void Sequencer::timerCallback()
{
    RelativeTime position(transport.getCurrentPosition());
    auto minutes = ((int)position.inMinutes()) % 60;
    auto seconds = ((int)position.inSeconds()) % 60;
    auto millis = ((int)position.inMilliseconds()) % 1000;
    auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    transportPosition.setText(positionString, dontSendNotification);
}

void Sequencer::changeListenerCallback(ChangeBroadcaster *)
{
    updatePlayPauseButtonText();
}

void Sequencer::togglePlayPause()
{
    if (transport.isPlaying())
    {
        transport.stop(false, false);
    }
    else
    {
        transport.play(false);
    }
}

void Sequencer::stop()
{
    transport.stop(false, false);
    transport.setCurrentPosition(0.0);
}

void Sequencer::updatePlayPauseButtonText()
{
    playPauseButton.setButtonText(transport.isPlaying() ? "Pause" : "Play");
}
