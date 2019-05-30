/*
  ==============================================================================

    FigureGenerator.cpp
    Created: 17 May 2019 4:37:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FigureGenerator.h"

//==============================================================================
FigureGenerator::FigureGenerator(ValueTree &as) : appState(as)
{
    holdingMessage.setText("holding area for figure generation options.", dontSendNotification);
    addAndMakeVisible(&holdingMessage);
}

FigureGenerator::~FigureGenerator()
{
}

void FigureGenerator::paint(Graphics &g)
{
}

void FigureGenerator::resized()
{
    auto area = getLocalBounds();
    holdingMessage.setBounds(area);
}

int FigureGenerator::getNewFigureId()
{
    auto figures = appState.getChildWithName(figuresIdentifier);
    int highestNumberId = 0;

    for (int i = 0; i < figures.getNumChildren(); i++)
    {
        int currentId = figures.getChild(i).getProperty(figurePropIdIdentifier);
        if (currentId > highestNumberId)
        {
            highestNumberId = currentId;
        }
    }

    return highestNumberId + 1;
}

ValueTree FigureGenerator::generateFigure()
{
    // NB: note that particles is NOT a member of the class and only gets particles from appState at the last minute, just before using it.
    // This is because of the difference between a copy and a reference. appState is a reference which will update, whereas a copy won't
    // so if you try and take a copy of particles from appState in the constructor, you will have an outdated version
    // copies as members work fine when you are using ValueTree listeners because you can update the member (copy) when you get a notification from the listener
    // but all you are really doing is the same as here - taking a copy from the reference to appState
    // particles here could be a reference but I can't work out how to initialize it properly
    auto particles = appState.getChildWithName(particlesIdentifier);
    auto particlesLength = particles.getNumChildren();
    if (particlesLength > 0) // flip this and make handle error
    {
        // create new figure locally (don't add it to appState yet)
        generatedFigure = ValueTree(figureIdentifier);
        auto newFigureId = getNewFigureId();
        generatedFigure.setProperty(figurePropIdIdentifier, newFigureId, nullptr);

        int figEventId = 1;
        double onset = 0.0;

        for (int i = 0; i < particlesLength; i++)
        {
            // for each particle, add a FigureEvent with an onset 1000ms more than the previous
            ValueTree figureEvent(figureEventIdentifier);
            figureEvent.setProperty(figureEventPropIdIdentifier, figEventId, nullptr);
            figureEvent.setProperty(figureEventPropOnsetIdentifier, onset, nullptr);
            figureEvent.setProperty(figureEventPropParticleIdIdentifier, particles.getChild(i).getProperty(particlePropIdIdentifier), nullptr);
            generatedFigure.addChild(figureEvent, -1, nullptr);
            figEventId++;
            onset += 1.0;
        }

        auto figures = appState.getChildWithName(figuresIdentifier);
        figures.addChild(generatedFigure, -1, nullptr); // this will need to check in future whether this figure already exists in figures
        return generatedFigure;
    }

    return ValueTree();
}
