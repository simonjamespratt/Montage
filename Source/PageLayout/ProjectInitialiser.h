#pragma once
#include "ProjectState.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ProjectInitialiser : public juce::Component {
  public:
    ProjectInitialiser(ProjectState &ps);
    void resized() override;

  private:
    ProjectState projectState;
    juce::Label explanationText;
    juce::TextButton createButton;
    juce::TextButton loadButton;
    void load();
    void create();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectInitialiser)
};
