#pragma once
#include "ProjectState.h"

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

class FiguresTable : public juce::Component, public juce::TableListBoxModel {
  public:
    FiguresTable(const ProjectState &ps);

    void resized() override;

    void setSelectedFigure(Figure f);
    void removeFigure();

    std::function<void(Figure f)> onFigureSelected;
    std::function<void()> onFigureDeselected;

    // TableListBoxModel overrides
    int getNumRows() override;
    void paintRowBackground(juce::Graphics &g,
                            int rowNumber,
                            int /*width*/,
                            int /*height*/,
                            bool rowIsSelected) override;
    void paintCell(juce::Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;
    void backgroundClicked(const juce::MouseEvent &) override;
    void selectedRowsChanged(int lastRowSelected) override;

  private:
    enum Columns : int { figureNum = 1, id = 2 };

    juce::Label heading;

    juce::TableListBox table;
    int numRows = 0;

    ProjectState projectState;
    FigureList figureList;

    void refreshTable();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FiguresTable)
};
