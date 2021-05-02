#include "ParticlesPage.h"

ParticlesPage::ParticlesPage(te::Engine &e, ProjectState &ps)
: particlesManager(ps, e),
  sourceManager(ps.getSourceList()),
  particlesManifest(ps.getParticleList())

{
    addAndMakeVisible(&sourceManager);
    addAndMakeVisible(&particlesManifest);
    addAndMakeVisible(&particlesManager);
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

    particlesManager.setBounds(area.removeFromLeft((int)colWidthUnit));
    sourceManager.setBounds(area.removeFromTop((int)rowHeightUnit));
    particlesManifest.setBounds(area);
}
