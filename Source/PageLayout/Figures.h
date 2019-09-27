/*
  ==============================================================================

    Figures.h
    Created: 7 May 2019 8:17:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Figures/FigureGenerator.h"
#include "../Sequencer/Sequencer.h"
#include "../DataManagement/FigureManager.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Figures : public Component
{
public:
    Figures(te::Engine &e, ValueTree &as);
    ~Figures();

    void paint(Graphics &) override;
    void resized() override;

private:
    Label heading;
    TextButton generateButton;
    te::Engine &engine;
    ValueTree &appState;
    Sequencer sequencer;
    FigureGenerator figureGenerator;
    FigureManager figureManager;
    void generateAndArrangeFigure();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
