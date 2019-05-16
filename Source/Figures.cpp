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
Figures::Figures(te::Engine &e) : engine(e), sequencer(engine)
{
    addAndMakeVisible(&sequencer);
}

Figures::~Figures()
{
}

void Figures::paint(Graphics &g)
{
}

void Figures::resized()
{
    sequencer.setBounds(getLocalBounds());
}
