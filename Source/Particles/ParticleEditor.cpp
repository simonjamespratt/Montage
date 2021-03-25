#include "ParticleEditor.h"

#include "ErrorMessageModal.h"

ParticleEditor::ParticleEditor(const Particle &p, te::Engine &eng)
: particle(p),
  model {particle.getStart(), particle.getEnd()},
  startEditor(model.start, "Start"),
  endEditor(model.end, "End"),
  edit(eng,
       /* TODO: TRACKTION: method signature for te::createEmptyEdit() is a
          legacy signature. Update to newer version. See
          tracktion_EditFileOperations.h */
       te::createEmptyEdit(eng),
       te::Edit::forEditing,
       nullptr,
       0),
  transport(edit.getTransport()),
  thumbnail(transport),
  cursor(transport, edit),
  transportController(transport),
  transportInteractor(transport, edit)
{
    name.setText(particle.getId().toString(), juce::dontSendNotification);
    addAndMakeVisible(name);

    startEditor.onChange = [this] {
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
        auto checkedEnd =
            particle.ensureNewEndIsWithinBounds(model.start, model.end);
        if(checkedEnd != model.end) {
            model.end = checkedEnd;
            endEditor.update();
        }

        transportInteractor.setSelectionRange({model.start, model.end});
    };
    addAndMakeVisible(&endEditor);

    prepAudio(eng);

    addAndMakeVisible(&thumbnail);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);
    transportInteractor.onSelectionChange = [this] {
        auto newRange = transportInteractor.getSelectionRange();
        model.start = newRange.rangeStart;
        model.end = newRange.rangeEnd;
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
}

void ParticleEditor::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    area.removeFromRight(5);
    area.removeFromLeft(5);
    area.removeFromBottom(10);
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

    auto transportArea = area.removeFromTop(50);
    transportArea.removeFromLeft(10);
    transportArea.removeFromRight(10);

    auto buttonControlsArea = area;

    // layout metadata ========================================
    name.setBounds(metaDataArea.removeFromLeft(300));
    startEditor.setBounds(metaDataArea.removeFromLeft(200));
    endEditor.setBounds(metaDataArea.removeFromLeft(200));

    // layout waveform ========================================
    thumbnail.setBounds(waveformArea);
    cursor.setBounds(waveformArea);
    transportInteractor.setBounds(waveformArea);

    // layout transport =======================================
    transportController.setBounds(transportArea);

    // layout buttons =========================================
    saveButton.setBounds(
        buttonControlsArea.removeFromLeft(100).reduced(margin));
    cancelButton.setBounds(
        buttonControlsArea.removeFromRight(100).reduced(margin));
}

// Private methods
void ParticleEditor::prepAudio(te::Engine &eng)
{
    auto file = particle.getSource().getFile();
    te::AudioFile audioFile(eng, file);

    thumbnail.setFile(audioFile, 0.0, audioFile.getLength());

    // prep edit:
    // clear clips from track
    auto track = te::getAudioTracks(edit)[0];
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

    // set up transport
    // NB: not sure these lines are actually needed
    transport.setLoopRange(newClip->getEditTimeRange());
    transport.looping = true;
    transport.position = 0.0;

    transportInteractor.setSelectionRange({model.start, model.end});
}
