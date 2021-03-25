#include "SourceSelector.h"

SourceSelector::SourceSelector(SourceList sl) : sourceList(sl)
{
    sourceList.onObjectAdded = [this](Source s) {
        configureSourceSelector();
    };
    sourceList.onObjectRemoved = [this](Source s) {
        configureSourceSelector();
    };

    helperText.setText("Choose a file: ", juce::dontSendNotification);
    addAndMakeVisible(helperText);

    configureSourceSelector();
    addAndMakeVisible(sourceSelector);

    confirmButton.setButtonText("Confirm");
    confirmButton.onClick = [this] {
        auto index = sourceSelector.getSelectedId() - 1;
        auto selectedSource = sourceList.getObjects()[index];
        if(onSourceChosen) {
            onSourceChosen(selectedSource);
        }
        configureSourceSelector();
    };
    addChildComponent(confirmButton);

    cancelButton.setButtonText("Cancel");
    cancelButton.onClick = [this] {
        if(onCancel) {
            onCancel();
        }
        configureSourceSelector();
    };
    addAndMakeVisible(cancelButton);
}

void SourceSelector::resized()
{
    auto area = getLocalBounds();
    helperText.setBounds(area.removeFromLeft(150).reduced(10));
    sourceSelector.setBounds(area.removeFromLeft(250).reduced(10));
    if(confirmButton.isVisible()) {
        confirmButton.setBounds(area.removeFromLeft(100).reduced(10));
    }
    cancelButton.setBounds(area.removeFromLeft(100).reduced(10));
}

// Private methods
void SourceSelector::configureSourceSelector()
{
    sourceSelector.clear(juce::dontSendNotification);
    sourceSelector.setTextWhenNothingSelected("--");
    // sourceSelector.addItem("Add new source", 999);
    int itemId = 1;
    auto sources = sourceList.getObjects();
    for(auto &source : sources) {
        sourceSelector.addItem(source.getFileName(), itemId);
        itemId++;
    }

    sourceSelector.onChange = [this] {
        if(sourceSelector.getSelectedId() != 0) {
            confirmButton.setVisible(true);
        } else {
            confirmButton.setVisible(false);
        }

        resized();
    };
}
