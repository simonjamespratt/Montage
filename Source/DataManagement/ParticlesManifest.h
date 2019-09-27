/*
  ==============================================================================

    ParticlesManifest.h
    Created: 27 Apr 2019 3:41:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Identifiers.h"

//==============================================================================
/*
*/
class ParticlesManifest : public Component, public TableListBoxModel, public ValueTree::Listener
{
public:
    ParticlesManifest(ValueTree &as);
    ~ParticlesManifest();

    void resized() override;

    // TableListBoxModel overrides
    int getNumRows() override;
    void paintRowBackground(Graphics &g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    // ValueTree change listeners
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override;
    void valueTreeChildAdded(ValueTree &parentTree, ValueTree &childWhichHasBeenAdded) override;
    void valueTreeChildRemoved(ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override;
    void valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved, int oldInex, int newIndex) override;
    void valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) override;

private:
    Label heading;
    ValueTree &appState;
    ValueTree particles;
    TableListBox table;
    std::array<Identifier, 4> dataTypes = {
        particlePropIdIdentifier,
        particlePropSourceIdIdentifier,
        particlePropRangeStartIdentifier,
        particlePropRangeEndIdentifier};
    int numRows = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesManifest)
};
