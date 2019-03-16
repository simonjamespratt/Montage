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
Sequencer::Sequencer() : engine(ProjectInfo::projectName)
{
    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");

    addAndMakeVisible(&playPauseButton);
    playPauseButton.setButtonText("Play");

    addAndMakeVisible(&editNameLabel);
    editNameLabel.setText("No edit loaded", dontSendNotification);

    addAndMakeVisible(&transportPosition);
    transportPosition.setText("0.00", dontSendNotification);

    // =========================================================================
    // NB: This section sets up the edit using a file
    // don't really understand how this works
    // see: https://github.com/Tracktion/tracktion_engine/blob/master/tutorials/PlaybackDemo.md
    const auto editFilePath = JUCEApplication::getCommandLineParameterArray()[0];
    jassert(editFilePath.isNotEmpty());
    const File editFile(editFilePath);

    if (editFile.existsAsFile())
    {
        // initialise the edit
        edit = std::make_unique<tracktion_engine::Edit>(
            engine,
            tracktion_engine::loadEditFromFile(editFile, {}),
            tracktion_engine::Edit::forEditing,
            nullptr,
            0);

        // initialise the transport - probably want to make this variable a class member in due course
        // auto &transport = edit->getTransport();
        // transport.looping = false;
        // transport.play(false);
        // transport.addChangeListener(this);

        // set up some other things
        editNameLabel.setText(editFile.getFileNameWithoutExtension(), dontSendNotification);
        // playPauseButton.onClick = [this] { EngineHelpers::togglePlay(*edit); };
    }
    else
    {
        jassertfalse;
    }
    // =========================================================================

    setSize(600, 400);
}

Sequencer::~Sequencer()
{
}

void Sequencer::resized()
{
    settingsButton.setBounds(10, 10, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 40, getWidth() - 20, 20);
    editNameLabel.setBounds(10, 70, getWidth() - 20, 20);
    transportPosition.setBounds(10, 100, getWidth() - 20, 20);
}

void Sequencer::changeListenerCallback(ChangeBroadcaster *source) {}
