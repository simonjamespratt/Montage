#pragma once

#include "ContentContainer.h"
#include "ProjectInitialiser.h"
#include "ProjectState.h"

class MainComponent : public juce::Component {
  public:
    MainComponent();
    ~MainComponent();

    void resized() override;

  private:
    ProjectState projectState;
    ProjectInitialiser projectInitialiser;
    ContentContainer contentContainer;
    juce::SharedResourcePointer<juce::TooltipWindow>
        tooltip; // creates a tooltip window that can be used by all components
                 // that use tooltips to display their messages, at any depth
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
