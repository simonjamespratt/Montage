#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParticleSelector.h"
#include "Sequencer.h"

namespace te = tracktion_engine;

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
  te::Engine engine;
  ParticleSelector particleSelector;
  Sequencer sequencer;

  TextButton settingsButton;

  void showAudioDeviceSettings(te::Engine &engine);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
