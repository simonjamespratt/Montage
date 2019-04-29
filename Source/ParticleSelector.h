/*
  ==============================================================================

    ParticleSelector.h
    Created: 11 Apr 2019 8:08:44pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Indentifiers.h"
#include "./TimeDisplay.h"
#include "./TracktionThumbnail.h"
#include "./Cursor.h"
#include "./TransportInteractor.h"
#include "./FileManager.h"
#include "./ErrorManager.h"
#include "./TransportController.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class ParticleSelector : public Component, public ChangeBroadcaster
{
public:
    ParticleSelector(te::Engine &eng, ValueTree &as);
    ~ParticleSelector();

    void resized() override;
    void paint(Graphics &g) override;

    bool readyToBeDeleted();

private:
    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
    // when this is working seriously, probably need to change this
    te::Edit edit;
    te::TransportControl &transport;

    ValueTree &appState;
    ValueTree sources;
    ValueTree particles;
    ValueTree source;
    ValueTree particle;

    bool toBeDeleted = false;

    TracktionThumbnail thumbnail;
    Cursor cursor;
    TransportInteractor transportInteractor;

    ComboBox sourceSelector;
    TransportController transportController;
    TextButton saveParticleButton;
    TextButton deleteParticleButton;

    Label particleNameDisplay;

    void initialiseSourceSelector();
    void sourceSelectorChanged();
    void selectNewSourceFile();
    void loadExistingSourceFile(int &id);
    int getNewParticleId();
    void saveParticle();
    void showErrorMessaging(const ErrorType &errorType);
    void addFileToEditAndLoop(File &file, te::AudioFile &audioFile);
    void deleteParticleSelector();
    void updateViewableData();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticleSelector)
};
