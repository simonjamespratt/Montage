#include "ParticleEditor.h"

#include "ErrorMessageModal.h"

ParticleEditor::ParticleEditor(const Particle &p, te::Engine &eng)
: particle(p),
  model {particle.getStart(), particle.getEnd(), particle.getName()},
  startEditor(model.start, "Start"),
  endEditor(model.end, "End"),
  nameEditor(model.name, "Name"),
  edit(te::createEmptyEdit(
      eng, juce::File())), // NB: We're not saving edits in ParticleEditor so
                           // just provide an empty (unvalid) juce::File
  transport(edit->getTransport()),
  transportManager(*edit),
  thumbnail(transport),
  cursor(*edit),
  transportController(transport),
  transportInteractor(*edit),
  timeScalingFactor {100, 100, 0, 1000},
  xZoom(juce::Slider::SliderStyle::LinearHorizontal,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    transportManager.onChange = [this] {
        thumbnailViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit->getLength());
    };
    transportManager.startManager();

    transportController.onTransportStopped = [this] {
        thumbnailViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit->getLength());
    };

    transportInteractor.onSelectionChangeInProgress =
        [this](const juce::MouseEvent event) {
            thumbnailViewport.syncToMouseDrag(event.getPosition());
        };

    startEditor.onChange = [this] {
        // when model start and end are both at 0 it denotes that there is no
        // current selection
        if(model.start == 0 && model.end == 0) {
            return;
        }
        auto checkedStart =
            particle.ensureNewStartIsWithinBounds(model.start, model.end);
        if(checkedStart != model.start) {
            model.start = checkedStart;
            startEditor.update();
        }

        transportInteractor.setSelectionRange({model.start, model.end});
    };
    addAndMakeVisible(&startEditor);

    endEditor.onChange = [this] {
        // when model start and end are both at 0 it denotes that there is no
        // current selection
        if(model.start == 0 && model.end == 0) {
            return;
        }
        auto checkedEnd =
            particle.ensureNewEndIsWithinBounds(model.start, model.end);
        if(checkedEnd != model.end) {
            model.end = checkedEnd;
            endEditor.update();
        }

        transportInteractor.setSelectionRange({model.start, model.end});
    };
    addAndMakeVisible(&endEditor);

    addAndMakeVisible(nameEditor);

    prepAudio(eng);

    thumbnailViewport.setViewedComponent(&thumbnailContainer, false);
    addAndMakeVisible(&thumbnailViewport);
    thumbnailViewport.setScrollBarsShown(false, true);

    thumbnailContainer.addAndMakeVisible(&thumbnail);
    thumbnailContainer.addAndMakeVisible(&cursor);
    thumbnailContainer.addAndMakeVisible(&transportInteractor);

    transportInteractor.onSelectionChanged = [this] {
        auto newRange = transportInteractor.getSelectionRange();
        model.start = newRange.start;
        model.end = newRange.end;
        startEditor.update();
        endEditor.update();
    };

    addAndMakeVisible(&transportController);

    saveButton.setButtonText("Save");
    saveButton.onClick = [this] {
        if(model.start != particle.getStart() &&
           model.end != particle.getEnd()) {
            try {
                particle.setStartAndEnd(model.start, model.end);
            } catch(const std::exception &e) {
                // Catches: ParticleStartInvalid & ParticleEndInvalid
                std::make_shared<ErrorMessageModal>(juce::String(e.what()));
                return;
            }
        }

        if(model.start != particle.getStart()) {
            try {
                particle.setStart(model.start);
            } catch(const std::exception &e) {
                // Catches: ParticleStartInvalid
                std::make_shared<ErrorMessageModal>(juce::String(e.what()));
                return;
            }
        }

        if(model.end != particle.getEnd()) {
            try {
                particle.setEnd(model.end);
            } catch(const std::exception &e) {
                // Catches: ParticleEndInvalid
                std::make_shared<ErrorMessageModal>(juce::String(e.what()));
                return;
            }
        }

        if(model.name != particle.getName()) {
            try {
                particle.setName(model.name);
            } catch(const std::exception &e) {
                // Catches ParticleNameInvalid
                std::make_shared<ErrorMessageModal>(juce::String(e.what()));
                return;
            }
        }

        if(onDoneEditing) {
            onDoneEditing();
        }
    };
    addAndMakeVisible(saveButton);

    cancelButton.setButtonText("Cancel");
    cancelButton.onClick = [this] {
        if(onDoneEditing) {
            onDoneEditing();
        }
    };
    addAndMakeVisible(cancelButton);

    xZoom.setRange(timeScalingFactor.min, timeScalingFactor.max);
    xZoom.setValue(timeScalingFactor.current);
    xZoom.onValueChange = [this] {
        timeScalingFactor.current = xZoom.getValue();
        resized();
    };
    addAndMakeVisible(xZoom);
}

void ParticleEditor::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    area.removeFromRight(5);
    area.removeFromLeft(5);
    area.removeFromBottom(5);
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(area);
}

void ParticleEditor::resized()
{
    // areas===================================================
    auto margin = 10;
    auto area = getLocalBounds();
    auto metaDataArea = area.removeFromTop(45);

    auto waveformArea = area.removeFromTop(160);
    waveformArea.removeFromRight(10);
    waveformArea.removeFromLeft(10);

    auto audioControlsArea = area.removeFromTop(50);
    audioControlsArea.removeFromLeft(10);
    audioControlsArea.removeFromRight(10);
    auto transportArea = audioControlsArea.reduced(200, 0);
    auto zoomControlsArea = audioControlsArea.removeFromRight(100);

    auto buttonControlsArea = area;

    // layout metadata ========================================
    nameEditor.setBounds(metaDataArea.removeFromLeft(300));
    startEditor.setBounds(metaDataArea.removeFromLeft(200));
    endEditor.setBounds(metaDataArea.removeFromLeft(200));

    // layout waveform ========================================
    thumbnailViewport.setBounds(waveformArea);
    auto editLength = edit->getLength();

    if(editLength > 0) {
        timeScalingFactor.min = thumbnailViewport.getWidth() / editLength;
        timeScalingFactor.max =
            (thumbnailViewport.getWidth() / editLength) * 10;
        xZoom.setRange(timeScalingFactor.min, timeScalingFactor.max);
    }

    auto thumbnailWidth = editLength * timeScalingFactor.current;
    auto thumbnailHeight = waveformArea.getHeight();

    thumbnailContainer.setSize(thumbnailWidth, thumbnailHeight);

    thumbnail.setSize(thumbnailWidth, thumbnailHeight);
    cursor.setSize(thumbnailWidth, thumbnailHeight);
    transportInteractor.setSize(thumbnailWidth, thumbnailHeight);

    // layout transport =======================================
    transportController.setBounds(transportArea);
    xZoom.setBounds(zoomControlsArea);

    // layout buttons =========================================
    saveButton.setBounds(
        buttonControlsArea.removeFromLeft(100).reduced(margin));
    cancelButton.setBounds(
        buttonControlsArea.removeFromRight(100).reduced(margin));
}

void ParticleEditor::setFocus()
{
    transportController.grabKeyboardFocus();
}

// Private methods
void ParticleEditor::prepAudio(te::Engine &eng)
{
    auto file = particle.getSource().getFile();
    te::AudioFile audioFile(eng, file);

    thumbnail.setFile(audioFile, 0.0, audioFile.getLength());

    // prep edit:
    // clear clips from track
    auto track = te::getAudioTracks(*edit)[0];
    jassert(track);

    auto clipsToRemove = track->getClips();
    for(int i = clipsToRemove.size(); --i >= 0;) {
        clipsToRemove.getUnchecked(i)->removeFromParentTrack();
    }

    // add new clip to track
    auto newClip = track->insertWaveClip(file.getFileNameWithoutExtension(),
                                         file,
                                         {{0.0, audioFile.getLength()}, 0.0},
                                         false);
    jassert(newClip);

    transportInteractor.setSelectionRange({model.start, model.end});
}
