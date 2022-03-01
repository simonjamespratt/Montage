#pragma once

#include "ProjectState.h"
#include "SequencerViewState.h"
#include "TransportController.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class SequencerControlsView : public juce::Component,
                              private juce::ValueTree::Listener {
  public:
    SequencerControlsView(SequencerViewState &s, te::Edit &e, ProjectState &ps);
    void resized() override;

  private:
    void valueTreePropertyChanged(juce::ValueTree &tree,
                                  const juce::Identifier &prop) override;

    SequencerViewState &sequencerViewState;
    te::Edit &edit;
    ProjectState projectState;

    juce::Slider trackControlPanelWidthAdjuster;
    TransportController transportController;
    juce::Slider xZoom;
    juce::Slider yZoom;
    juce::TextButton renderButton {"Render"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencerControlsView)
};
