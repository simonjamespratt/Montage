/*
  ==============================================================================

    Sequencer.h
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Sequencer : public Component, public ChangeListener, public Timer
{
  public:
    Sequencer();
    ~Sequencer();

    void resized() override;
    void timerCallback() override;

  private:
    tracktion_engine::Engine engine{ProjectInfo::projectName};
    // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
    // when the sequencer is working seriously, probably need to change this
    tracktion_engine::Edit edit{engine, tracktion_engine::createEmptyEdit(), tracktion_engine::Edit::forEditing, nullptr, 0};
    tracktion_engine::TransportControl &transport{edit.getTransport()};

    TextButton playPauseButton;
    TextButton stopButton;
    Label transportPosition;

    void changeListenerCallback(ChangeBroadcaster *) override;
    void togglePlayPause();
    void stop();
    void updatePlayPauseButtonText();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
