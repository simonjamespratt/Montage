#pragma once
#include "EventList.h"
#include "ProjectState.h"

#include <juce_data_structures/juce_data_structures.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class FigureManager : public juce::Component, public juce::TableListBoxModel {
  public:
    FigureManager();
    ~FigureManager();

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

    void setData(const Figure &f, const ProjectState &ps);
    void clear();

  private:
    enum Columns : int {
        eventNum = 1,
        id = 2,
        onset = 3,
        particleStart = 4,
        particleEnd = 5,
        fileName = 6
    };

    std::unique_ptr<EventList> eventList;

    juce::TableListBox table;
    int numRows = 0;

    juce::Label heading;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureManager)
};
