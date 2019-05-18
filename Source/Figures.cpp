/*
  ==============================================================================

    Figures.cpp
    Created: 7 May 2019 8:17:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Figures.h"

//==============================================================================
Figures::Figures(te::Engine &e, ValueTree &as) : engine(e), appState(as), sequencer(engine, appState), figureGenerator(appState), figureManager(appState)
{
    addAndMakeVisible(&figureGenerator);
    addAndMakeVisible(&figureManager);
    addAndMakeVisible(&sequencer);

    heading.setText("Figures", dontSendNotification);
    heading.setFont(Font(24.0f, Font::bold));
    addAndMakeVisible(&heading);

    generateButton.setButtonText("Generate figure");
    addAndMakeVisible(&generateButton);
    generateButton.onClick = [this] { generateAndArrangeFigure(); };

}

Figures::~Figures()
{
}

void Figures::paint(Graphics &g)
{
}

void Figures::resized()
{
    auto area = getLocalBounds();

    auto headingArea = area.removeFromTop(50);
    heading.setBounds(headingArea);

    auto genButtonArea = area.removeFromTop(30);
    generateButton.setBounds(genButtonArea);

    auto heightUnit = area.getHeight() / 3;
    auto figureControlArea = area.removeFromTop(heightUnit);
    figureGenerator.setBounds(figureControlArea.removeFromLeft(100));
    figureManager.setBounds(figureControlArea);

    sequencer.setBounds(area);
}

void Figures::generateAndArrangeFigure()
{
    ValueTree figure = figureGenerator.generateFigure();
    sequencer.readFigure(figure);
}
