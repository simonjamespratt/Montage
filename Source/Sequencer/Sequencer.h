#pragma once

#include "Arrangement.h"
#include "AudioEditViewport.h"
#include "Figure.h"
#include "Identifiers.h"
#include "ProjectState.h"
#include "SequencerControlsView.h"
#include "SequencerViewState.h"
#include "Timeline.h"
#include "TrackControlPanel.h"
#include "TransportManager.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class Sequencer : public juce::Component, private juce::ValueTree::Listener {
  public:
    explicit Sequencer(std::unique_ptr<te::Edit> e, Figure f, ProjectState &ps);
    ~Sequencer();

    void resized() override;

    static std::unique_ptr<te::Edit> createEdit(Figure figure,
                                                const ProjectState &ps,
                                                juce::File file,
                                                te::Engine &e);

  private:
    void valueTreePropertyChanged(juce::ValueTree &tree,
                                  const juce::Identifier &prop) override;

    void save();

    std::unique_ptr<te::Edit> edit;
    ProjectState projectState;
    SequencerViewState sequencerViewState;

    te::TransportControl &transport;
    TransportManager transportManager;

    AudioEditViewport timelineViewport;
    Timeline timeline;

    AudioEditViewport arrangementViewport;
    Arrangement arrangement;

    AudioEditViewport panelsViewport;
    TrackControlPanel panels;

    SequencerControlsView controlsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
