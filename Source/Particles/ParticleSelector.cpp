#include "ParticleSelector.h"

ParticleSelector::ParticleSelector(te::Engine &eng, juce::ValueTree &as)
: engine(eng),
  edit(engine,
       /* TODO: TRACKTION: method signature for te::createEmptyEdit() is a
          legacy signature. Update to newer version. See
          tracktion_EditFileOperations.h */
       te::createEmptyEdit(engine),
       te::Edit::forEditing,
       nullptr,
       0),
  transport(edit.getTransport()),
  appState(as),
  sources(),
  thumbnail(transport),
  cursor(transport, edit),
  transportInteractor(transport, edit),
  transportController(transport)
{
    sources = (appState.getChildWithName(IDs::SOURCES));
    particles = (appState.getChildWithName(IDs::PARTICLES));

    addAndMakeVisible(&particleNameDisplay);
    particleNameDisplay.setFont(juce::Font(20.0f, juce::Font::bold));
    updateViewableData();

    addAndMakeVisible(&sourceSelector);
    initialiseSourceSelector();
    sourceSelector.onChange = [this] {
        sourceSelectorChanged();
    };

    addAndMakeVisible(&saveParticleButton);
    saveParticleButton.setButtonText("Save particle");
    saveParticleButton.onClick = [this] {
        saveParticle();
    };

    addAndMakeVisible(&deleteParticleButton);
    deleteParticleButton.setButtonText("Delete particle");
    deleteParticleButton.onClick = [this] {
        deleteParticleSelector();
    };

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

void ParticleSelector::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    area.removeFromRight(5);
    area.removeFromLeft(5);
    area.removeFromBottom(10);
    g.setColour(juce::Colours::darkgrey);
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

    juce::FlexBox dataDisplayFb;
    dataDisplayFb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    dataDisplayFb.alignItems = juce::FlexBox::AlignItems::center;
    dataDisplayFb.items.add(juce::FlexItem(particleNameDisplay)
                                .withHeight(20.0f)
                                .withWidth(100.0f)
                                .withMargin(juce::FlexItem::Margin(10.0f)));
    dataDisplayFb.items.add(juce::FlexItem(sourceSelector)
                                .withHeight(20.0f)
                                .withWidth(200.0f)
                                .withMargin(juce::FlexItem::Margin(10.0f)));
    dataDisplayFb.performLayout(dataDisplayArea.toFloat());

    thumbnail.setBounds(waveformArea);
    cursor.setBounds(waveformArea);
    transportInteractor.setBounds(waveformArea);
    transportController.setBounds(transportArea);

    juce::FlexBox persistanceActionsFb;
    persistanceActionsFb.justifyContent =
        juce::FlexBox::JustifyContent::flexStart;
    persistanceActionsFb.items.add(
        juce::FlexItem(saveParticleButton)
            .withHeight(20.f)
            .withWidth(100.0f)
            .withMargin(juce::FlexItem::Margin(10.0f)));
    persistanceActionsFb.items.add(
        juce::FlexItem(deleteParticleButton)
            .withHeight(20.f)
            .withWidth(100.0f)
            .withMargin(juce::FlexItem::Margin(10.0f)));
    persistanceActionsFb.performLayout(persistanceArea.toFloat());
}

void ParticleSelector::initialiseSourceSelector()
{
    sourceSelector.setTextWhenNothingSelected("--");
    sourceSelector.addItem("Add new source", 999);

    for(int i = 0; i < sources.getNumChildren(); i++) {
        auto source = sources.getChild(i);
        sourceSelector.addItem(source[IDs::file_name], source[IDs::id]);
    }
}

void ParticleSelector::sourceSelectorChanged()
{
    auto selectedId = sourceSelector.getSelectedId();

    if(selectedId == 999) {
        selectNewSourceFile();
    } else {
        loadExistingSourceFile(selectedId);
    }
}

void ParticleSelector::selectNewSourceFile()
{
    FileManager fileManager;
    fileManager.chooseFile();

    if(!fileManager.fileIsValid(engine)) {
        showErrorMessaging(FileInvalid);
        sourceSelector.setSelectedId(0);
        return;
    }

    // if the fileapth already exists, error and return
    const auto filePath = fileManager.getFile().getFullPathName();
    const auto existingEntry =
        sources.getChildWithProperty(IDs::file_path, filePath);
    if(existingEntry.isValid()) {
        showErrorMessaging(FileAlreadyExists);
        return;
    }

    source = fileManager.addSourceToState(sources);
    sourceSelector.addItem(source[IDs::file_name], source[IDs::id]);
    sourceSelector.setSelectedId(source[IDs::id]);

    juce::File file = fileManager.getFile();
    te::AudioFile audioFile = fileManager.getAudioFile(engine);
    addFileToEditAndLoop(file, audioFile);
}

void ParticleSelector::loadExistingSourceFile(int &id)
{
    auto requestedSource = sources.getChildWithProperty(IDs::id, id);

    if(requestedSource.isValid()) {
        FileManager fileManager;
        fileManager.loadExistingSourceFile(requestedSource);

        if(!fileManager.fileIsValid(engine)) {
            showErrorMessaging(FileInvalid);
            sourceSelector.setSelectedId(0);
            return;
        }

        source = requestedSource;
        sourceSelector.setSelectedId(source[IDs::id]);

        juce::File file = fileManager.getFile();
        te::AudioFile audioFile = fileManager.getAudioFile(engine);
        addFileToEditAndLoop(file, audioFile);
    }
}

int ParticleSelector::getNewParticleId()
{
    int highestNumberId = 0;

    for(int i = 0; i < particles.getNumChildren(); i++) {
        int currentId = particles.getChild(i).getProperty(IDs::id);
        if(currentId > highestNumberId) {
            highestNumberId = currentId;
        }
    }

    return highestNumberId + 1;
}

void ParticleSelector::saveParticle()
{
    auto isUpdate = particle.isValid();

    // check there is a valid source
    if(!source.isValid()) {
        showErrorMessaging(SourceInvalid);
        return;
    }
    // check selection range to ensure is valid
    auto particleRange = transportInteractor.getSelectionRange();
    if(!(particleRange.rangeEnd > 0.0)) {
        showErrorMessaging(ParticleRangeInvalid);
        return;
    }

    if(!isUpdate) {
        // if doesn't exist, make new vt for particle
        auto newId = getNewParticleId();
        particle = juce::ValueTree(IDs::PARTICLE);
        particle.setProperty(IDs::id, newId, nullptr);
    }

    particle.setProperty(IDs::source_id, source[IDs::id], nullptr);
    particle.setProperty(IDs::start, particleRange.rangeStart, nullptr);
    particle.setProperty(IDs::end, particleRange.rangeEnd, nullptr);

    if(!isUpdate) {
        // if doesn't exist, add new entry to the particles vt, BUT only AFTER
        // adding all required props otherwise will throw exception in Particle
        // class
        particles.addChild(particle, -1, nullptr);
    }

    // NB: on success, display the details of the particle entry somewhere
    // within this object
    updateViewableData();
}

void ParticleSelector::showErrorMessaging(const ErrorType &errorType)
{
    std::make_shared<ErrorManager>(errorType);
}

void ParticleSelector::addFileToEditAndLoop(juce::File &file,
                                            te::AudioFile &audioFile)
{
    // clear clips from track
    auto track = te::getAudioTracks(edit)[0];

    if(!track) {
        return;
    }

    auto clipsToRemove = track->getClips();
    for(int i = clipsToRemove.size(); --i >= 0;) {
        clipsToRemove.getUnchecked(i)->removeFromParentTrack();
    }

    // add new clip to track
    auto newClip = track->insertWaveClip(file.getFileNameWithoutExtension(),
                                         file,
                                         {{0.0, audioFile.getLength()}, 0.0},
                                         false);

    if(!newClip) {
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
    if(particle.isValid()) {
        particleNameDisplay.setText(particle.getProperty(IDs::id).toString(),
                                    juce::dontSendNotification);
    } else {
        particleNameDisplay.setText("Untitled", juce::dontSendNotification);
    }
}
