#pragma once

#include "Identifiers.h"
#include "Figure.h"

#include <juce_gui_basics/juce_gui_basics.h>

class FigureGenerator : public juce::Component {
  public:
    FigureGenerator(juce::ValueTree &as);
    ~FigureGenerator();

    void paint(juce::Graphics &) override;
    void resized() override;

    Figure generateFigure();

  private:
    juce::ValueTree &appState;
    juce::ValueTree generatedFigure;

    // TODO: replace with UI for fig gen options
    juce::Label holdingMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureGenerator)
};
