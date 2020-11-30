#pragma once

#include "Cursor.h"
#include "ErrorManager.h"
#include "FileManager.h"
#include "Identifiers.h"
#include "TracktionThumbnail.h"
#include "TransportController.h"
#include "TransportInteractor.h"

namespace te = tracktion_engine;

class ParticleSelector :
public juce::Component,
    public juce::ChangeBroadcaster {
  public:
    ParticleSelector(te::Engine &eng, juce::ValueTree &as);
    ~ParticleSelector();

    void resized() override;
    void paint(juce::Graphics &g) override;

    bool readyToBeDeleted();

  private:
    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by
    // referencing a file to write to when this is working seriously, probably
    // need to change this
    te::Edit edit;
    te::TransportControl &transport;

    // NB: need a ValueTree listener in this class for capturing changes to
    // sources then refresh the view for the combobox to display the updated
    // version of sources
    juce::ValueTree &appState;
    juce::ValueTree sources;
    juce::ValueTree particles;
    juce::ValueTree source;
    juce::ValueTree particle;

    bool toBeDeleted = false;

    TracktionThumbnail thumbnail;
    Cursor cursor;
    TransportInteractor transportInteractor;

    juce::ComboBox sourceSelector;
    TransportController transportController;
    juce::TextButton saveParticleButton;
    juce::TextButton deleteParticleButton;

    juce::Label particleNameDisplay;

    void initialiseSourceSelector();
    void sourceSelectorChanged();
    void selectNewSourceFile();
    void loadExistingSourceFile(int &id);
    int getNewParticleId();
    void saveParticle();
    void showErrorMessaging(const ErrorType &errorType);
    void addFileToEditAndLoop(juce::File &file, te::AudioFile &audioFile);
    void deleteParticleSelector();
    void updateViewableData();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticleSelector)
};
