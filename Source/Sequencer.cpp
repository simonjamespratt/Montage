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
Sequencer::Sequencer(te::Engine &eng) : engine(eng),
                                        edit(engine, te::createEmptyEdit(), te::Edit::forEditing, nullptr, 0),
                                        transport(edit.getTransport()),
                                        timeline(edit),
                                        cursor(transport, edit),
                                        arrangement(edit, transport),
                                        transportPosition(transport),
                                        transportInteractor(transport, edit)
{
    addAndMakeVisible(&loadFileButton);
    loadFileButton.setButtonText("Load file");
    loadFileButton.onClick = [this] { selectAudioFile(); };

    transport.addChangeListener(this);

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonText();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&transportPosition);
    addAndMakeVisible(&timeline);
    addAndMakeVisible(&arrangement);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);

    setSize(600, 400);
}

Sequencer::~Sequencer()
{
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::resized()
{
    loadFileButton.setBounds(10, 10, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 70, getWidth() - 20, 20);
    stopButton.setBounds(10, 100, getWidth() - 20, 20);
    transportPosition.setBounds(10, 130, getWidth() - 20, 20);
    timeline.setBounds(10, 160, getWidth() - 20, 20);
    arrangement.setBounds(10, 190, getWidth() - 20, 200);
    cursor.setBounds(10, 190, getWidth() - 20, 200);
    transportInteractor.setBounds(10, 190, getWidth() - 20, 200);
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

        te::AudioFile audioFile(file);

        if (!audioFile.isValid())
        {
            return;
        }

        arrangement.addClipToTrack(file, 1, 1.0, 3.5, 0.25);

        timeline.recalculate();
        transport.position = 0.0;
        transport.play(false);
    }
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
