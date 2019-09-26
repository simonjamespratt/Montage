/*
  ==============================================================================

    ContentContainer.h
    Created: 29 Apr 2019 8:01:36pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./MainHeader.h"
#include "./Figures.h"
#include "./ParticlesPage.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class ContentContainer : public Component
{
public:
    ContentContainer(ValueTree &as);
    ~ContentContainer();

    void paint(Graphics &) override;
    void resized() override;

private:
    ValueTree &appState;
    te::Engine engine;
    MainHeader mainHeader;

    TabbedComponent pages;
    Figures figures;
    ParticlesPage particlesPage;

    Rectangle<int> screenSize = Desktop::getInstance().getDisplays().getMainDisplay().userArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContentContainer)
};
