#pragma once
#include "ProjectState.h"

#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

class MainHeader : public juce::Component {
  public:
    MainHeader(te::Engine &e, const ProjectState &ps);
    ~MainHeader();

    void resized() override;

  private:
    te::Engine &engine;
    ProjectState state;

    juce::TextButton settingsButton;

    // project state save / load / create
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton createButton;

    void showAudioDeviceSettings(te::Engine &engine);
    void refreshView(ProjectState::Status status);
    void load();
    void create();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainHeader)
};
