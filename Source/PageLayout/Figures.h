#pragma once

#include "EventsTable.h"
#include "FigureGenerator.h"
#include "FiguresTable.h"
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
    FiguresTable figuresTable;
    EventsTable eventsTable;
    void arrangeFigure(Figure f);

    juce::Label heading;
    juce::DrawableButton addFigureButton;
    juce::DrawableButton deleteFigureButton;
    bool showGenerator {false};
    juce::TextButton closeGeneratorButton;
    void toggleGenerateManagerState();
    void refreshView();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
