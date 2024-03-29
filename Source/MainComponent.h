#pragma once

#include "ContentContainer.h"
#include "ProjectState.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component {
  public:
    // constructor and deconstructor
    MainComponent();
    ~MainComponent();

    void resized() override;

  private:
    ProjectState projectState;
    ContentContainer contentContainer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
