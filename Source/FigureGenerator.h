/*
  ==============================================================================

    FigureGenerator.h
    Created: 17 May 2019 4:37:12pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Indentifiers.h"
#include "./NumberGenerators.h"

//==============================================================================
/*
*/
class FigureGenerator : public Component
{
public:
    FigureGenerator(ValueTree &as);
    ~FigureGenerator();

    void paint(Graphics &) override;
    void resized() override;

    ValueTree generateFigure();

private:
    ValueTree &appState;
    ValueTree generatedFigure;
    Label holdingMessage;
    TextButton generateButton;
    Label randomNumberDisplay;
    TextButton getRandomNumberButton;
    void getRandomNumber();

    int getNewFigureId();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureGenerator)
};
