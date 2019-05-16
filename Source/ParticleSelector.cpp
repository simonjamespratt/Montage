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
                                                                     thumbnail(transport),
                                                                     cursor(transport, edit),
                                                                     transportInteractor(transport, edit),
                                                                     transportController(transport)
{
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    
    addAndMakeVisible(&particleNameDisplay);
    particleNameDisplay.setFont(Font(20.0f, Font::bold));
    updateViewableData();

    addAndMakeVisible(&sourceSelector);
    initialiseSourceSelector();
    sourceSelector.onChange = [this] { sourceSelectorChanged(); };

    addAndMakeVisible(&saveParticleButton);
    saveParticleButton.setButtonText("Save particle");
    saveParticleButton.onClick = [this] { saveParticle(); };

    addAndMakeVisible(&deleteParticleButton);
    deleteParticleButton.setButtonText("Delete particle");
    deleteParticleButton.onClick = [this] { deleteParticleSelector(); };

    addAndMakeVisible(&thumbnail);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);
    addAndMakeVisible(&transportController);
}

ParticleSelector::~ParticleSelector()
{
    edit.getTempDirectory(false).deleteRecursively();
    particles.removeChild(particle, nullptr);
}

void ParticleSelector::paint(Graphics &g)
{
    auto area = getLocalBounds();
    area.removeFromRight(5);
    area.removeFromLeft(5);
    area.removeFromBottom(10);
    g.setColour(Colours::darkgrey);
    g.fillRect(area);
}

void ParticleSelector::resized()
{
    //    total height of particle: 290
    auto area = getLocalBounds();
    auto dataDisplayArea = area.removeFromTop(40);
    auto waveformArea = area.removeFromTop(160);
    waveformArea.removeFromRight(10);
    waveformArea.removeFromLeft(10);
    auto transportArea = area.removeFromTop(50);
    transportArea.removeFromLeft(10);
    transportArea.removeFromRight(10);
    auto persistanceArea = area;
    
    FlexBox dataDisplayFb;
    dataDisplayFb.justifyContent = FlexBox::JustifyContent::spaceBetween;
    dataDisplayFb.alignItems = FlexBox::AlignItems::center;
    dataDisplayFb.items.add(FlexItem(particleNameDisplay).withHeight(20.0f).withWidth(100.0f).withMargin(FlexItem::Margin(10.0f)));
    dataDisplayFb.items.add(FlexItem(sourceSelector).withHeight(20.0f).withWidth(200.0f).withMargin(FlexItem::Margin(10.0f)));
    dataDisplayFb.performLayout(dataDisplayArea.toFloat());

    thumbnail.setBounds(waveformArea);
    cursor.setBounds(waveformArea);
    transportInteractor.setBounds(waveformArea);
    transportController.setBounds(transportArea);
    
    FlexBox persistanceActionsFb;
    persistanceActionsFb.justifyContent = FlexBox::JustifyContent::flexStart;
    persistanceActionsFb.items.add(FlexItem(saveParticleButton).withHeight(20.f).withWidth(100.0f).withMargin(FlexItem::Margin(10.0f)));
    persistanceActionsFb.items.add(FlexItem(deleteParticleButton).withHeight(20.f).withWidth(100.0f).withMargin(FlexItem::Margin(10.0f)));
    persistanceActionsFb.performLayout(persistanceArea.toFloat());
}

void ParticleSelector::initialiseSourceSelector()
{
    sourceSelector.setTextWhenNothingSelected("--");
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
    updateViewableData();
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

void ParticleSelector::deleteParticleSelector()
{
    toBeDeleted = true;
    sendChangeMessage();
}

bool ParticleSelector::readyToBeDeleted()
{
    return toBeDeleted;
}

void ParticleSelector::updateViewableData()
{
    if (particle.isValid()) {
        particleNameDisplay.setText(particle.getProperty(particlePropIdIdentifier).toString(), dontSendNotification);
    }
    else {
        particleNameDisplay.setText("Untitled", dontSendNotification);
    }
    
}
