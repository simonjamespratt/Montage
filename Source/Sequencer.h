/*
  ==============================================================================

    Sequencer.h
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Timeline.h"
#include "./Cursor.h"
#include "./Arrangement.h"
#include "./TransportInteractor.h"
#include "./TransportController.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Sequencer : public Component
{
public:
    Sequencer(te::Engine &eng);
    ~Sequencer();

    void paint(Graphics &g) override;
    void resized() override;

private:
    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
    // when the sequencer is working seriously, probably need to change this
    te::Edit edit;
    te::TransportControl &transport;

    Timeline timeline;
    Arrangement arrangement;
    Cursor cursor;
    TransportInteractor transportInteractor;
    TransportController transportController;

    TextButton loadFileButton; // TODO: to be deleted

    void selectAudioFile();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
