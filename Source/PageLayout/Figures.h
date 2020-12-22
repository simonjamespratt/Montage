#pragma once

#include "FigureGenerator.h"
#include "FigureManager.h"
#include "Sequencer.h"

namespace te = tracktion_engine;

class Figures : public juce::Component {
  public:
    Figures(te::Engine &e, juce::ValueTree &as);
    ~Figures();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    te::Engine &engine;
    juce::ValueTree &appState;
    Sequencer sequencer;
    FigureGenerator figureGenerator;
    FigureManager figureManager;
    void generateAndArrangeFigure();

    juce::Label heading;
    bool showGenerator {true};
    juce::TextButton toggleGeneratOutputButton;
    void toggleGenerateManagerState();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
