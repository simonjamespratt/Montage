#pragma once

#include "Figures.h"
#include "MainHeader.h"
#include "ParticlesPage.h"
#include "ProjectState.h"

namespace te = tracktion_engine;

class ContentContainer : public juce::Component {
  public:
    ContentContainer(ProjectState &ps);
    ~ContentContainer();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    te::Engine engine;
    MainHeader mainHeader;

    juce::TabbedComponent pages;
    Figures figures;
    ParticlesPage particlesPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentContainer)
};
