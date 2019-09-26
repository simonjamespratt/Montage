/*
  ==============================================================================

    ParticleSelector.h
    Created: 11 Apr 2019 8:08:44pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Identifiers.h"
#include "../AudioComponents/TracktionThumbnail.h"
#include "../AudioComponents/Cursor.h"
#include "../AudioComponents/TransportInteractor.h"
#include "../DataManagement/FileManager.h"
#include "../Errors/ErrorManager.h"
#include "../AudioComponents/TransportController.h"

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

    // NB: need a ValueTree listener in this class for capturing changes to sources
    // then refresh the view for the combobox to display the updated version of sources
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
