#pragma once

#include "Arrangement.h"
#include "Cursor.h"
#include "Figure.h"
#include "Identifiers.h"
#include "ProjectState.h"
#include "Timeline.h"
#include "TransportController.h"
#include "TransportInteractor.h"

#include <functional>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

struct ClipData {
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> clip;
    int trackIndex;
    double clipStart;
    double clipEnd;
    double offset;
};

// NB: Provides solution for keeping two viewports aligned as per
// https://forum.juce.com/t/how-to-fix-a-region-in-viewport/36095/11
class ViewportWithCallback : public juce::Viewport {
  public:
    void
    visibleAreaChanged(const juce::Rectangle<int> &newVisibleArea) override;
    std::function<void(const juce::Rectangle<int> &newVisibleArea)>
        onVisibleAreaChanged;
};

class Sequencer : public juce::Component {
  public:
    Sequencer(te::Engine &eng);
    ~Sequencer();

    void resized() override;

    void readFigure(const Figure &figure, const ProjectState &projectState);
    void clear();

  private:
    int noOfTracks;
    float timeScalingFactor;
    float trackHeight;

    te::Engine &engine;
    // NB: note that the edit is set up with en empty edit rather than by
    // referencing a file to write to when the sequencer is working seriously,
    // probably need to change this
    te::Edit edit;
    te::TransportControl &transport;
    te::LambdaTimer transportReporter;

    Timeline timeline;
    ViewportWithCallback timelineViewport;

    ViewportWithCallback arrangementContainerViewport;
    juce::Component arrangementContainer;
    Arrangement arrangement;
    Cursor cursor;
    TransportInteractor transportInteractor;

    TransportController transportController;

    juce::Slider xZoom;
    juce::Slider yZoom;

    void prepareForNewFigure(int noOfParticles);
    void clearTracks();
    void prepareTracks();
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip>
    addClipToTrack(const juce::File &file,
                   const int trackIndex,
                   const double &clipStart,
                   const double &clipEnd,
                   const double &offset);
    double
    getTransportPositionWithinComponent(const juce::Component &component);
    void syncViewportToTransportPosition();
    void syncViewportToMousePosition(const juce::MouseEvent event);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
