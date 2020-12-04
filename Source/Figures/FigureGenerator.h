#pragma once

#include "Identifiers.h"
#include "NumberGenerators.h"

#include <juce_gui_basics/juce_gui_basics.h>

class FigureGenerator : public juce::Component {
  public:
    FigureGenerator(juce::ValueTree &as);
    ~FigureGenerator();

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::ValueTree generateFigure();

  private:
    juce::ValueTree &appState;
    juce::ValueTree generatedFigure;
    juce::Label holdingMessage;
    juce::TextButton generateButton;
    juce::Label randomNumberDisplay;
    juce::TextButton getRandomNumberButton;
    void getRandomNumber();

    int getNewFigureId();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureGenerator)
};
