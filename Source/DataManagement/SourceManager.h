/*
  ==============================================================================

    SourceManager.h
    Created: 22 Apr 2019 7:30:41pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Identifiers.h"
#include "./FileManager.h"
#include "../Errors/ErrorManager.h"
#include "../Utilities/Icons.h"

//==============================================================================
/*
*/
class SourceManager : public Component, public TableListBoxModel, public ValueTree::Listener
{
public:
    SourceManager(ValueTree &as);
    ~SourceManager();

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
    ValueTree sources;
    ValueTree particles;
    TableListBox table;
    Icons icons;
    DrawablePath crossIcon;
    DrawableButton addSourceFileButton;
    DrawablePath dashIcon;
    DrawableButton deleteSourceFilesButton;
    std::array<Identifier, 3> dataTypes = {sourcePropIdIdentifier, sourcePropFileNameIdentifier, sourcePropFilePathIdentifier};
    int numRows = 0;

    void selectNewSourceFile();
    void showErrorMessaging(const ErrorType &errorType);
    void deleteSources();
    bool sourceIsInUse(int index);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SourceManager)
};
