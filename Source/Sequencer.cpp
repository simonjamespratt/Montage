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
Sequencer::Sequencer()
{
    startTimer(20);
    transport.addChangeListener(this);

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonText();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&transportPosition);

    setSize(600, 400);
}

Sequencer::~Sequencer()
{
    stopTimer();
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::resized()
{
    playPauseButton.setBounds(10, 10, getWidth() - 20, 20);
    stopButton.setBounds(10, 40, getWidth() - 20, 20);
    transportPosition.setBounds(10, 70, getWidth() - 20, 20);
}

void Sequencer::timerCallback()
{
    // RelativeTime position(transport.getCurrentPosition());
    // auto minutes = ((int)position.inMinutes()) % 60;
    // auto seconds = ((int)position.inSeconds()) % 60;
    // auto millis = ((int)position.inMilliseconds()) % 1000;
    // auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);
    // transportPosition.setText(positionString, dontSendNotification);
    transportPosition.setText(String(transport.getCurrentPosition()), dontSendNotification);
    // DBG(transport.getCurrentPosition());
}

void Sequencer::changeListenerCallback(ChangeBroadcaster *)
{
    updatePlayPauseButtonText();
}

void Sequencer::togglePlayPause()
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

void Sequencer::stop()
{
    transport.stop(false, false);
    transport.setCurrentPosition(0.0);
}

void Sequencer::updatePlayPauseButtonText()
{
    playPauseButton.setButtonText(transport.isPlaying() ? "Pause" : "Play");
}
