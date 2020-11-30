#include "ParticlesPage.h"

ParticlesPage::ParticlesPage(juce::ValueTree &as, te::Engine &e)
: appState(as),
  engine(e),
  particlesContainer(engine, appState),
  sourceManager(appState, engine),
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
{}

void ParticlesPage::paint(juce::Graphics &g)
{}

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
