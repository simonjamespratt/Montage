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
ParticleSelector::ParticleSelector(te::Engine &eng, ValueTree &as) : engine(eng),
                                                                     edit(engine, te::createEmptyEdit(), te::Edit::forEditing, nullptr, 0),
                                                                     transport(edit.getTransport()),
                                                                     appState(as),
                                                                     sources(),
                                                                     transportPosition(transport),
                                                                     thumbnail(transport),
                                                                     cursor(transport, edit),
                                                                     transportInteractor(transport, edit)
{
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    transport.addChangeListener(this);

    addAndMakeVisible(&sourceSelector);
    initialiseSourceSelector();
    sourceSelector.onChange = [this] { sourceSelectorChanged(); };

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonText();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&saveParticleButton);
    saveParticleButton.setButtonText("Save particle");
    saveParticleButton.onClick = [this] { saveParticle(); };

    addAndMakeVisible(&deleteParticleButton);
    deleteParticleButton.setButtonText("Delete particle");
    deleteParticleButton.onClick = [this] { deleteParticleSelector(); };

    addAndMakeVisible(&transportPosition);
    addAndMakeVisible(&thumbnail);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);
}

ParticleSelector::~ParticleSelector()
{
    edit.getTempDirectory(false).deleteRecursively();
    particles.removeChild(particle, nullptr);
}

void ParticleSelector::resized()
{
    sourceSelector.setBounds(10, 10, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    transportPosition.setBounds(10, 100, getWidth() - 20, 20);

    thumbnail.setBounds(10, 130, getWidth() - 20, getHeight() - 160);
    cursor.setBounds(10, 130, getWidth() - 20, getHeight() - 160);
    transportInteractor.setBounds(10, 130, getWidth() - 20, getHeight() - 160);

    saveParticleButton.setBounds(10, getHeight() - 20, 100, 20);
    deleteParticleButton.setBounds(110, getHeight() - 20, 100, 20);
}

void ParticleSelector::initialiseSourceSelector()
{
    sourceSelector.setTextWhenNothingSelected("No source file currently chosen");
    sourceSelector.addItem("Add new source", 999);

    for (int i = 0; i < sources.getNumChildren(); i++)
    {
        auto source = sources.getChild(i);
        sourceSelector.addItem(source[sourcePropFileNameIdentifier], source[sourcePropIdIdentifier]);
    }
}

void ParticleSelector::sourceSelectorChanged()
{
    auto selectedId = sourceSelector.getSelectedId();

    if (selectedId == 999)
    {
        selectNewSourceFile();
    }
    else
    {
        loadExistingSourceFile(selectedId);
    }
}

void ParticleSelector::selectNewSourceFile()
{
    FileManager fileManager;
    fileManager.chooseFile();

    if (!fileManager.fileIsValid())
    {
        showErrorMessaging(FileInvalid);
        sourceSelector.setSelectedId(0);
        return;
    }

    // if the fileapth already exists, error and return
    const auto filePath = fileManager.getFile().getFullPathName();
    const auto existingEntry = sources.getChildWithProperty(sourcePropFilePathIdentifier, filePath);
    if (existingEntry.isValid())
    {
        showErrorMessaging(FileAlreadyExists);
        return;
    }

    source = fileManager.addSourceToState(sources);
    sourceSelector.addItem(source[sourcePropFileNameIdentifier], source[sourcePropIdIdentifier]);
    sourceSelector.setSelectedId(source[sourcePropIdIdentifier]);

    File file = fileManager.getFile();
    te::AudioFile audioFile = fileManager.getAudioFile();
    addFileToEditAndLoop(file, audioFile);
}

void ParticleSelector::loadExistingSourceFile(int &id)
{
    auto requestedSource = sources.getChildWithProperty(sourcePropIdIdentifier, id);

    if (requestedSource.isValid())
    {
        FileManager fileManager;
        fileManager.loadExistingSourceFile(requestedSource);

        if (!fileManager.fileIsValid())
        {
            showErrorMessaging(FileInvalid);
            sourceSelector.setSelectedId(0);
            return;
        }

        source = requestedSource;
        sourceSelector.setSelectedId(source[sourcePropIdIdentifier]);

        File file = fileManager.getFile();
        te::AudioFile audioFile = fileManager.getAudioFile();
        addFileToEditAndLoop(file, audioFile);
    }
}

int ParticleSelector::getNewParticleId()
{
    int highestNumberId = 0;

    for (int i = 0; i < particles.getNumChildren(); i++)
    {
        int currentId = particles.getChild(i).getProperty(particlePropIdIdentifier);
        if (currentId > highestNumberId)
        {
            highestNumberId = currentId;
        }
    }

    return highestNumberId + 1;
}

void ParticleSelector::saveParticle()
{
    // check there is a valid source
    if (!source.isValid())
    {
        showErrorMessaging(SourceInvalid);
        return;
    }
    // check selection range to ensure is valid
    auto particleRange = transportInteractor.getSelectionRange();
    if (!(particleRange.rangeEnd > 0.0))
    {
        showErrorMessaging(ParticleRangeInvalid);
        return;
    }

    if (!particle.isValid())
    {
        // if doesn't exist, add new entry to the particles vt
        auto newId = getNewParticleId();
        particle = ValueTree(particleIdentifier);
        particle.setProperty(particlePropIdIdentifier, newId, nullptr);
        particles.addChild(particle, -1, nullptr);
    }

    particle.setProperty(particlePropSourceIdIdentifier, source[sourcePropIdIdentifier], nullptr);
    particle.setProperty(particlePropRangeStartIdentifier, particleRange.rangeStart, nullptr);
    particle.setProperty(particlePropRangeEndIdentifier, particleRange.rangeEnd, nullptr);

    // TODO: on success, display the details of the particle entry somewhere within this object
}

void ParticleSelector::showErrorMessaging(const ErrorType &errorType)
{
    std::make_shared<ErrorManager>(errorType);
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
        {{0.0, audioFile.getLength()}, 0.0},
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

void ParticleSelector::deleteParticleSelector()
{
    toBeDeleted = true;
    sendChangeMessage();
}

bool ParticleSelector::readyToBeDeleted()
{
    return toBeDeleted;
}
