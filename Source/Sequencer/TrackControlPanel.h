#pragma once
#include "ParticleList.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Panel : public juce::Component {
  public:
    Panel(juce::String trackNumber, juce::String trackName);
    void resized() override;
    void paint(juce::Graphics &g) override;

  private:
    juce::Label number;
    juce::Label name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Panel)
};

class TrackControlPanel : public juce::Component {
  public:
    TrackControlPanel(float initialTrackHeight);
    void resized() override;
    void paint(juce::Graphics &g) override;
    void createPanels(ParticleList particleList);
    void clear();
    void setTrackHeight(float newHeight);

  private:
    std::vector<std::unique_ptr<Panel>> panels;
    float trackHeight;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackControlPanel)
};
