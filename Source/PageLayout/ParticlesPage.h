#pragma once

#include "ParticlesSelection.h"
#include "ParticlesManifest.h"
#include "SourceManager.h"

namespace te = tracktion_engine;

class ParticlesPage : public juce::Component {
  public:
    ParticlesPage(juce::ValueTree &as, te::Engine &e);
    ~ParticlesPage();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    juce::ValueTree &appState;
    te::Engine &engine;
    juce::Viewport particlesViewport;
    ParticlesSelection particlesContainer;
    SourceManager sourceManager;
    ParticlesManifest particlesManifest;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesPage)
};
