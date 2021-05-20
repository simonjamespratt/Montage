#include "ParticlesPage.h"

ParticlesPage::ParticlesPage(te::Engine &e, ProjectState &ps)
: particlesManager(ps, e), sourceManager(ps.getSourceList())
{
    addAndMakeVisible(&sourceManager);
    addAndMakeVisible(&particlesManager);
}

ParticlesPage::~ParticlesPage()
{}

void ParticlesPage::paint(juce::Graphics &g)
{}

void ParticlesPage::resized()
{
    auto area = getLocalBounds();
    auto colWidthUnit = area.getWidth() / 2.0;

    particlesManager.setBounds(area.removeFromLeft((int)colWidthUnit));
    sourceManager.setBounds(area);
}
