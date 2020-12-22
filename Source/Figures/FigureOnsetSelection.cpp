#include "FigureOnsetSelection.h"

FigureOnsetSelection::FigureOnsetSelection(
    std::shared_ptr<aleatoric::DurationsProducer> durationsProducer,
    DurationProtocolParams durationProtocolParams)
: numberProtocolSelector(durationsProducer),
  durationProtocolSelector(durationsProducer, durationProtocolParams)
{
    heading.setText("Onset selection", juce::dontSendNotification);
    addAndMakeVisible(&heading);
    heading.setFont(juce::Font(20.0f, juce::Font::bold));

    addAndMakeVisible(&numberProtocolSelector);
    addAndMakeVisible(&durationProtocolSelector);
}

FigureOnsetSelection::~FigureOnsetSelection()
{}

void FigureOnsetSelection::paint(juce::Graphics &g)
{}

void FigureOnsetSelection::resized()
{
    auto area = getLocalBounds();
    auto colWidth = area.getWidth() / 2;
    heading.setBounds(area.removeFromTop(50));
    durationProtocolSelector.setBounds(area.removeFromLeft(colWidth));
    numberProtocolSelector.setBounds(area);
}
