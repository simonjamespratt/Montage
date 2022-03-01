#pragma once

#include "EventsTable.h"
#include "FigureGenerator.h"
#include "FigureList.h"
#include "FiguresTable.h"
#include "ProjectState.h"
#include "Sequencer.h"

namespace te = tracktion_engine;

class Figures : public juce::Component {
  public:
    Figures(te::Engine &e, ProjectState &ps);

    void resized() override;

  private:
    void arrangeFigure(Figure f);
    void toggleGenerateManagerState();
    void refreshView();

    te::Engine &engine;
    ProjectState projectState;
    FigureList figureList;

    std::unique_ptr<Sequencer> sequencer;
    FigureGenerator figureGenerator;
    FiguresTable figuresTable;
    EventsTable eventsTable;

    juce::Label heading;
    juce::DrawableButton addFigureButton;
    juce::DrawableButton deleteFigureButton;
    bool showGenerator {false};
    juce::TextButton closeGeneratorButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Figures)
};
