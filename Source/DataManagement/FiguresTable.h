#pragma once
#include "ProjectState.h"
#include "SettingsReview.h"

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
    juce::Component *
    refreshComponentForCell(int rowNumber,
                            int columnId,
                            bool /*isRowSelected*/,
                            Component *existingComponentToUpdate) override;

    void setText(const int columnNumber,
                 const int rowNumber,
                 const juce::String &newText);

    juce::String getText(const int columnNumber, const int rowNumber) const;

    Figure getFigureByRow(int rowNumber) const;

    juce::TableListBox table;

  private:
    enum Columns : int {
        figureNum = 1,
        name = 2,
        id = 3,
        showCreationSettings = 4
    };

    juce::Label heading;

    int numRows = 0;

    ProjectState projectState;
    FigureList figureList;

    void refreshTable();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FiguresTable)
};

class EditableCell : public juce::Label {
  public:
    EditableCell(FiguresTable &ft);
    void mouseDown(const juce::MouseEvent &event) override;
    void textWasEdited() override;
    void setRowAndColumn(const int newRow, const int newColumn);

  private:
    FiguresTable &owner;
    int row;
    int columnId;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditableCell)
};

class FigureSettings : public juce::Component {
  public:
    FigureSettings(Figure f);
    void resized() override;

  private:
    SettingsReview durationsReview;
    SettingsReview durationsSelectionsReview;
    SettingsReview particleSelectionsReview;
    juce::Label noContentMessage {"", "No information to display"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureSettings)
};

class CreationSettingsCell : public juce::Component {
  public:
    CreationSettingsCell(FiguresTable &ft);
    void resized() override;
    void setRowAndColumn(const int newRow, const int newColumn);

  private:
    void showSettingsModal(Figure f);

    FiguresTable &owner;
    juce::TextButton btn {"View"};
    int row;
    int columnId;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CreationSettingsCell)
};
