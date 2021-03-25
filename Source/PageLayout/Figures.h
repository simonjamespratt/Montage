#pragma once

#include "FigureGenerator.h"
#include "FigureManager.h"
#include "ProjectState.h"
#include "Sequencer.h"

namespace te = tracktion_engine;

class Figures : public juce::Component {
  public:
    Figures(te::Engine &e, ProjectState &ps);
    ~Figures();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    te::Engine &engine;
    ProjectState &projectState;

    Sequencer sequencer;
    FigureGenerator figureGenerator;
    FigureManager figureManager;
    void arrangeFigure(Figure f);

    juce::Label heading;
    bool showGenerator {true};
    juce::TextButton toggleGeneratOutputButton;
    void toggleGenerateManagerState();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
