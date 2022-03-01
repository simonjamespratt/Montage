#pragma once
#include "AutoParamValueSource.h"
#include "Helpers.h"
#include "LevelMeterView.h"
#include "ParticleList.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class Panel : public juce::Component,
              private juce::AsyncUpdater,
              private juce::ValueTree::Listener {
  public:
    Panel(te::AudioTrack::Ptr t);
    ~Panel();

    void resized() override;
    void paint(juce::Graphics &g) override;

  private:
    // AsyncUpdater
    void handleAsyncUpdate() override;

    // ValueTree::Listener
    void valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c) override;
    void
    valueTreeChildRemoved(juce::ValueTree &p, juce::ValueTree &c, int) override;
    void valueTreePropertyChanged(juce::ValueTree &tree,
                                  const juce::Identifier &prop) override;

    void showPluginsModal();
    void populateAutomationParams();

    te::AudioTrack::Ptr track;
    juce::Label number;
    juce::Label name;
    juce::TextButton fxButton {"FX"};
    juce::ComboBox autoParamSelector;
    juce::DrawableButton clearAutomationBtn;
    LevelMeterView levelMeterView;
    juce::Slider volumeSlider;
    juce::Slider panSlider;
    juce::TextButton muteButton {"M"};
    juce::TextButton soloButton {"S"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Panel)
};

class TrackControlPanel : public juce::Component,
                          private juce::AsyncUpdater,
                          private juce::ValueTree::Listener {
  public:
    TrackControlPanel(te::Edit &e);
    ~TrackControlPanel();
    void resized() override;
    void paint(juce::Graphics &g) override;

  private:
    // AsyncUpdater
    void handleAsyncUpdate() override;

    // ValueTree::Listener
    void valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c) override;
    void
    valueTreeChildRemoved(juce::ValueTree &p, juce::ValueTree &c, int) override;

    void createPanels();

    te::Edit &edit;
    juce::OwnedArray<Panel> panels;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackControlPanel)
};
