/*
  ==============================================================================

    ParticleSelector.cpp
    Created: 11 Apr 2019 8:08:44pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParticleSelector.h"

//==============================================================================
ParticleSelector::ParticleSelector(te::Engine &eng) : engine(eng),
                                                      edit(engine, te::createEmptyEdit(), te::Edit::forEditing, nullptr, 0),
                                                      transport(edit.getTransport()),
                                                      transportPosition(transport),
                                                      thumbnail(transport),
                                                      cursor(transport, edit),
                                                      transportInteractor(transport, edit)
{
    transport.addChangeListener(this);

    addAndMakeVisible(&loadFileButton);
    loadFileButton.setButtonText("Load file");
    loadFileButton.onClick = [this] { selectAudioFile(); };

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonText();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&transportPosition);
    addAndMakeVisible(&thumbnail);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);

}

ParticleSelector::~ParticleSelector()
{
    edit.getTempDirectory(false).deleteRecursively();
}

void ParticleSelector::resized()
{
    loadFileButton.setBounds(10, 10, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    transportPosition.setBounds(10, 100, getWidth() - 20, 20);
    thumbnail.setBounds(10, 130, getWidth() - 20, getHeight() - 130);
    cursor.setBounds(10, 130, getWidth() - 20, getHeight() - 130);
    transportInteractor.setBounds(10, 130, getWidth() - 20, getHeight() - 130);
}

void ParticleSelector::selectAudioFile()
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

        addFileToEditAndLoop(file, audioFile);
    }
}

void ParticleSelector::addFileToEditAndLoop(File &file, te::AudioFile &audioFile)
{
    // clear clips from track
    auto track = edit.getOrInsertAudioTrackAt(0);

    if (!track)
    {
        return;
    }

    auto clipsToRemove = track->getClips();
    for (int i = clipsToRemove.size(); --i >= 0;)
    {
        clipsToRemove.getUnchecked(i)->removeFromParentTrack();
    }

    // add new clip to track
    auto newClip = track->insertWaveClip(
        file.getFileNameWithoutExtension(),
        file,
        { { 0.0, audioFile.getLength() }, 0.0 },
        false);

    if (!newClip)
    {
        return;
    }

    // set the thumbnail with the new clip
    thumbnail.setFile(audioFile, 0.0, audioFile.getLength());

    // set up transport
    transport.setLoopRange(newClip->getEditTimeRange());
    transport.looping = true;
    transport.position = 0.0;
    transport.play(false);

}

void ParticleSelector::updatePlayPauseButtonText()
{
    playPauseButton.setButtonText(transport.isPlaying() ? "Pause" : "Play");
}

void ParticleSelector::changeListenerCallback(ChangeBroadcaster *)
{
    updatePlayPauseButtonText();
}

void ParticleSelector::togglePlayPause()
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

void ParticleSelector::stop()
{
    transport.stop(false, false);
    transport.setCurrentPosition(0.0);
}
