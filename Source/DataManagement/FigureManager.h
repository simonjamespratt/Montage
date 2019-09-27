/*
  ==============================================================================

    FigureManager.h
    Created: 20 May 2019 8:57:13am
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Identifiers.h"

//==============================================================================
/*
*/
class FigureManager : public Component, public TableListBoxModel, public ValueTree::Listener
{
public:
    FigureManager(ValueTree &as);
    ~FigureManager();

    void paint(Graphics &) override;
    void resized() override;

    // TableListBoxModel overrides
    int getNumRows() override;
    void paintRowBackground(Graphics &g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void backgroundClicked(const MouseEvent &) override;

    // ValueTree change listeners
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override;
    void valueTreeChildAdded(ValueTree &parentTree, ValueTree &childWhichHasBeenAdded) override;
    void valueTreeChildRemoved(ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved, int oldInex, int newIndex) override;
    void valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) override;

private:
    Label heading;
    ValueTree &appState;
    ValueTree figures;
    ValueTree figure;
    TableListBox table;
    std::array<Identifier, 3> dataTypes = {
        figureEventPropIdIdentifier,
        figureEventPropOnsetIdentifier,
        figureEventPropParticleIdIdentifier};
    int numRows = 0;

    void setData();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureManager)
};
