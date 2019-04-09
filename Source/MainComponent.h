#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Source.h"
#include "Sequencer.h"

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component
{
public:
  // constructor and deconstructor
  MainComponent();
  ~MainComponent();
  void resized() override;

private:
  // Source source;
  Sequencer sequencer;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
