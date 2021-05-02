#pragma once

#include "Icons.h"
#include "SourceList.h"

class SourceManager : public juce::Component, public juce::TableListBoxModel {
  public:
    SourceManager(SourceList sl);

    void resized() override;

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

  private:
    enum Columns : int { id = 1, name = 2, path = 3, length = 4 };

    SourceList sourceList;

    juce::TableListBox table;
    int numRows = 0;

    juce::Label heading;
    Icons icons;
    juce::DrawablePath crossIcon;
    juce::DrawablePath dashIcon;
    juce::DrawableButton addSourceButton;
    juce::DrawableButton deleteSourceButton;

    void selectNewSourceFile();
    void addSource(juce::File &file);
    void deleteSource();
    void refreshView();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SourceManager)
};
