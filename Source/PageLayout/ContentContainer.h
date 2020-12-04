#pragma once

#include "Figures.h"
#include "MainHeader.h"
#include "ParticlesPage.h"

namespace te = tracktion_engine;

class ContentContainer : public juce::Component {
  public:
    ContentContainer(juce::ValueTree &as);
    ~ContentContainer();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    juce::ValueTree &appState;
    te::Engine engine;
    MainHeader mainHeader;

    juce::TabbedComponent pages;
    Figures figures;
    ParticlesPage particlesPage;

    juce::Rectangle<int> screenSize =
        juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentContainer)
};
