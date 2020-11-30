#pragma once
#include "Identifiers.h"

#include <array>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_gui_basics/juce_gui_basics.h>

class FigureManager : public juce::Component,
                      public juce::TableListBoxModel,
                      public juce::ValueTree::Listener {
  public:
    FigureManager(juce::ValueTree &as);
    ~FigureManager();

    void paint(juce::Graphics &) override;
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

    // ValueTree change listeners
    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged,
                                  const juce::Identifier &property) override;
    void valueTreeChildAdded(juce::ValueTree &parentTree,
                             juce::ValueTree &childWhichHasBeenAdded) override;
    void valueTreeChildRemoved(juce::ValueTree &parentTree,
                               juce::ValueTree &childWhichHasBeenRemoved,
                               int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged(
        juce::ValueTree &parentTreeWhoseChildrenHaveMoved,
        int oldInex,
        int newIndex) override;
    void
    valueTreeParentChanged(juce::ValueTree &treeWhoseParentHasChanged) override;

  private:
    juce::Label heading;
    juce::ValueTree &appState;
    juce::ValueTree figures;
    juce::ValueTree figure;
    juce::TableListBox table;
    std::array<juce::Identifier, 3> dataTypes {
        figureEventPropIdIdentifier,
        figureEventPropOnsetIdentifier,
        figureEventPropParticleIdIdentifier};
    int numRows = 0;

    void setData();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureManager)
};
