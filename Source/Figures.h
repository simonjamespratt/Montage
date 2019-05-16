/*
  ==============================================================================

    Figures.h
    Created: 7 May 2019 8:17:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Sequencer.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Figures : public Component
{
public:
    Figures(te::Engine &e);
    ~Figures();

    void paint(Graphics &) override;
    void resized() override;

private:
    te::Engine &engine;
    Sequencer sequencer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
