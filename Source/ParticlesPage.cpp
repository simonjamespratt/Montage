/*
  ==============================================================================

    ParticlesPage.cpp
    Created: 7 May 2019 8:38:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParticlesPage.h"

//==============================================================================
ParticlesPage::ParticlesPage(ValueTree &as, te::Engine &e) : appState(as),
                                                            engine(e),
                                                            particlesContainer(engine, appState),
                                                            sourceManager(appState),
                                                            particlesManifest(appState)

{
    addAndMakeVisible(&sourceManager);
    addAndMakeVisible(&particlesManifest);
    addAndMakeVisible(&particlesContainer);

    particlesViewport.setViewedComponent(&particlesContainer, false);
    particlesViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&particlesViewport);
}

ParticlesPage::~ParticlesPage()
{
}

void ParticlesPage::paint(Graphics &g)
{
}

void ParticlesPage::resized()
{
    auto area = getLocalBounds();
    auto rowHeightUnit = area.getHeight() / 2.0;
    auto colWidthUnit = area.getWidth() / 2.0;

    auto particlesArea = area.removeFromLeft((int)colWidthUnit);
    particlesViewport.setBounds(particlesArea);
    particlesContainer.setBounds(particlesArea);

    sourceManager.setBounds(area.removeFromTop((int)rowHeightUnit));
    particlesManifest.setBounds(area);

}
