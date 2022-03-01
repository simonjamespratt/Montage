#pragma once

#include "AutoParamGraph.h"
#include "Cursor.h"
#include "Helpers.h"
#include "SequencerViewState.h"
#include "TracktionThumbnail.h"
#include "TransportInteractor.h"

#include <memory>

namespace te = tracktion_engine;

class ArrangementTrack : public juce::Component,
                         private juce::ValueTree::Listener,
                         private FlaggedAsyncUpdater {
  public:
    ArrangementTrack(te::AudioTrack::Ptr t);
    ~ArrangementTrack();
    void resized() override;
    void paint(juce::Graphics &g) override;

  private:
    void valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c) override;
    void
    valueTreeChildRemoved(juce::ValueTree &p, juce::ValueTree &c, int) override;
    void valueTreePropertyChanged(juce::ValueTree &tree,
                                  const juce::Identifier &prop) override;
    void handleAsyncUpdate() override;

    void createThumbnails();

    te::AudioTrack::Ptr track;
    juce::OwnedArray<AudioClipView> clipViews;
    std::unique_ptr<AutoParamGraph> autoParamGraph;

    bool updateClips = false;
    bool updateAutoParamGraph = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArrangementTrack)
};

class Arrangement : public juce::Component,
                    private juce::ValueTree::Listener,
                    private juce::AsyncUpdater {
  public:
    Arrangement(te::Edit &e, SequencerViewState &vs);
    ~Arrangement();

    void resized() override;

  private:
    void valueTreePropertyChanged(juce::ValueTree &,
                                  const juce::Identifier &prop) override;
    void valueTreeChildAdded(juce::ValueTree &parent,
                             juce::ValueTree &child) override;
    void valueTreeChildRemoved(juce::ValueTree &parent,
                               juce::ValueTree &child,
                               int) override;

    void handleAsyncUpdate() override;

    void createArrangementTracks();

    te::Edit &edit;
    SequencerViewState &sequencerViewState;
    juce::OwnedArray<ArrangementTrack> tracks;

    Cursor cursor;
    TransportInteractor transportInteractor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
