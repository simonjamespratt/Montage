#pragma once

#include "ErrorManager.h"
#include "FileManager.h"
#include "Icons.h"
#include "Identifiers.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class SourceManager : public juce::Component,
                      public juce::TableListBoxModel,
                      public juce::ValueTree::Listener {
  public:
    SourceManager(juce::ValueTree &as, te::Engine &eng);
    ~SourceManager();

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
    te::Engine &engine;
    juce::ValueTree sources;
    juce::ValueTree particles;
    juce::TableListBox table;
    Icons icons;
    juce::DrawablePath crossIcon;
    juce::DrawableButton addSourceFileButton;
    juce::DrawablePath dashIcon;
    juce::DrawableButton deleteSourceFilesButton;
    std::array<juce::Identifier, 3> dataTypes = {
        IDs::id, IDs::file_name, IDs::file_path};
    int numRows = 0;

    void selectNewSourceFile();
    void showErrorMessaging(const ErrorType &errorType);
    void deleteSources();
    bool sourceIsInUse(int index);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SourceManager)
};
