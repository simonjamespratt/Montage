#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Indentifiers.h"
#include "./ParticleSelector.h"
#include "./Sequencer.h"
#include "./Particles.h"
#include "./SourceManager.h"
#include "./ParticlesManifest.h"

namespace te = tracktion_engine;

/*
ValueTree type Identifier: AppState
    child ValueTree type Identifier: Sources
        child ValueTree type Identifier:Source
            id: int
            filePath: string
            fileName: string

    child ValueTree type Identifier: Particles
        child ValueTree type Identifier: Particle
            id: int
            sourceId: int
            rangeStart:double
            rangeEnd: double
*/

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

  ValueTree appState;

private:
  te::Engine engine;
  SourceManager sourceManager;
  ParticlesManifest particlesManifest;
  Particles particlesContainer;
  // ParticleSelector particleSelector;
  // Sequencer sequencer;

  TextButton settingsButton;

  void showAudioDeviceSettings(te::Engine &engine);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
