#pragma once
#include "Identifiers.h"

#include <array>
#include <juce_gui_basics/juce_gui_basics.h>

class ParticlesManifest :
public juce::Component,
    public juce::TableListBoxModel,
    public juce::ValueTree::Listener {
  public:
    ParticlesManifest(juce::ValueTree &as);
    ~ParticlesManifest();

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
    juce::ValueTree particles;
    juce::TableListBox table;
    std::array<juce::Identifier, 4> dataTypes = {
        particlePropIdIdentifier,
        particlePropSourceIdIdentifier,
        particlePropRangeStartIdentifier,
        particlePropRangeEndIdentifier};
    int numRows = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesManifest)
};
