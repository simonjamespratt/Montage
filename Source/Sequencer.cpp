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
Sequencer::Sequencer() : engine(ProjectInfo::projectName),
                         edit(engine, tracktion_engine::createEmptyEdit(), tracktion_engine::Edit::forEditing, nullptr, 0),
                         transport(edit.getTransport())
{

    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");

    addAndMakeVisible(&playPauseButton);
    playPauseButton.setButtonText("Play");

    addAndMakeVisible(&transportPosition);
    // NB: this needs doing properly - see Source class
    transportPosition.setText("0.00", dontSendNotification);

    setSize(600, 400);
}

Sequencer::~Sequencer()
{
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::resized()
{
    settingsButton.setBounds(10, 10, getWidth() - 20, 20);
    playPauseButton.setBounds(10, 40, getWidth() - 20, 20);
    transportPosition.setBounds(10, 70, getWidth() - 20, 20);
}

void Sequencer::changeListenerCallback(ChangeBroadcaster *source) {}
