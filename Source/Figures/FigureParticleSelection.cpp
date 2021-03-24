#include "FigureParticleSelection.h"

FigureParticleSelection::FigureParticleSelection(
    std::shared_ptr<aleatoric::CollectionsProducer<Particle>> particleProducer)
: selector(particleProducer)
{
    heading.setText("Particle selection", juce::dontSendNotification);
    addAndMakeVisible(&heading);
    heading.setFont(juce::Font(20.0f, juce::Font::bold));

    addAndMakeVisible(&selector);
}

FigureParticleSelection::~FigureParticleSelection()
{}

void FigureParticleSelection::paint(juce::Graphics &g)
{}

void FigureParticleSelection::resized()
{
    auto area = getLocalBounds();
    heading.setBounds(area.removeFromTop(50));
    selector.setBounds(area);
}

void FigureParticleSelection::resetParams()
{
    selector.resetParams();
}
