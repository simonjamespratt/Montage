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
#include "./Indentifiers.h"
#include "./FileManager.h"
#include "./ErrorManager.h"

namespace te = tracktion_engine;

struct ClipData
{
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> clip;
    int trackIndex;
    double clipStart;
    double clipEnd;
    double offset;
};

//==============================================================================
/*
*/
class Sequencer : public Component
{
public:
    Sequencer(te::Engine &eng, ValueTree &as);
    ~Sequencer();

    void paint(Graphics &g) override;
    void resized() override;

    void readFigure(ValueTree &figure);

private:
    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by referencing a file to write to
    // when the sequencer is working seriously, probably need to change this
    te::Edit edit;
    te::TransportControl &transport;

    ValueTree &appState;

    Timeline timeline;
    Arrangement arrangement;
    Cursor cursor;
    TransportInteractor transportInteractor;
    TransportController transportController;
    int noOfTracks;
    void showErrorMessaging(const ErrorType &errorType);
    void prepareForNewFigure(int noOfParticles);
    void prepareTracks();
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> addClipToTrack(const File &file, const int trackIndex, const double &clipStart, const double &clipEnd, const double &offset);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
