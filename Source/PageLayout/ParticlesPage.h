#pragma once

#include "ParticlesManager.h"
#include "ParticlesManifest.h"
#include "ProjectState.h"
#include "SourceManager.h"

namespace te = tracktion_engine;

class ParticlesPage : public juce::Component {
  public:
    ParticlesPage(te::Engine &e, ProjectState &ps);
    ~ParticlesPage();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    ParticlesManager particlesManager;
    SourceManager sourceManager;
    ParticlesManifest particlesManifest;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesPage)
};
