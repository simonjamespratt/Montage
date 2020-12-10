#include "Figures.h"

Figures::Figures(te::Engine &e, juce::ValueTree &as)
: engine(e),
  appState(as),
  sequencer(engine, appState),
  figureGenerator(appState),
  figureManager(appState)
{
    addAndMakeVisible(&figureGenerator);
    addAndMakeVisible(&figureManager);
    addAndMakeVisible(&sequencer);

    heading.setText("Figures", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    generateButton.setButtonText("Generate figure");
    addAndMakeVisible(&generateButton);
    generateButton.onClick = [this] {
        generateAndArrangeFigure();
    };
}

Figures::~Figures()
{}

void Figures::paint(juce::Graphics &g)
{}

void Figures::resized()
{
    auto area = getLocalBounds();

    auto headingArea = area.removeFromTop(50);
    heading.setBounds(headingArea);

    auto genButtonArea = area.removeFromTop(30);
    generateButton.setBounds(genButtonArea);

    auto heightUnit = area.getHeight() / 3;
    auto figureControlArea = area.removeFromTop(heightUnit);
    figureGenerator.setBounds(figureControlArea.removeFromLeft(400));
    figureManager.setBounds(figureControlArea);

    sequencer.setBounds(area);
}

void Figures::generateAndArrangeFigure()
{
    auto figure = figureGenerator.generateFigure();
    sequencer.readFigure(figure);
}
