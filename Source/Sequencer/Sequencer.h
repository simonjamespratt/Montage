#pragma once

#include "Arrangement.h"
#include "Cursor.h"
#include "Figure.h"
#include "Identifiers.h"
#include "ProjectState.h"
#include "Timeline.h"
#include "TransportController.h"
#include "TransportInteractor.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

struct ClipData {
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> clip;
    int trackIndex;
    double clipStart;
    double clipEnd;
    double offset;
};

class Sequencer : public juce::Component {
  public:
    Sequencer(te::Engine &eng);
    ~Sequencer();

    void resized() override;

    void readFigure(const Figure &figure, const ProjectState &projectState);
    void clear();

  private:
    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by
    // referencing a file to write to when the sequencer is working seriously,
    // probably need to change this
    te::Edit edit;
    te::TransportControl &transport;

    Timeline timeline;
    Arrangement arrangement;
    Cursor cursor;
    TransportInteractor transportInteractor;
    TransportController transportController;
    int noOfTracks;
    void prepareForNewFigure(int noOfParticles);
    void clearTracks();
    void prepareTracks();
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip>
    addClipToTrack(const juce::File &file,
                   const int trackIndex,
                   const double &clipStart,
                   const double &clipEnd,
                   const double &offset);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
