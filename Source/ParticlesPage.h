/*
  ==============================================================================

    ParticlesPage.h
    Created: 7 May 2019 8:38:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Particles.h"
#include "./SourceManager.h"
#include "./ParticlesManifest.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class ParticlesPage : public Component
{
public:
    ParticlesPage(ValueTree &as, te::Engine &e);
    ~ParticlesPage();

    void paint(Graphics &) override;
    void resized() override;

private:
    ValueTree &appState;
    te::Engine &engine;
    Viewport particlesViewport;
    Particles particlesContainer;
    SourceManager sourceManager;
    ParticlesManifest particlesManifest;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesPage)
};
