#include "StringEditorWithLabel.h"

StringEditorWithLabel::StringEditorWithLabel(juce::String &editableValue,
                                             juce::String labelText,
                                             int labelSpace)
: value(editableValue), labelGutter(labelSpace)
{
    input.setText(value);
    input.setJustification(juce::Justification::centredLeft);
    input.onTextChange = [this] {
        value = input.getText();
    };
    input.onReturnKey = [this] {
        getTopLevelComponent()->grabKeyboardFocus();
    };

    label.setText(labelText + ": ", juce::dontSendNotification);
    label.attachToComponent(&input, true);

    addAndMakeVisible(input);
    addAndMakeVisible(label);
}

void StringEditorWithLabel::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    area.removeFromLeft(labelGutter); // make space for the label
    input.setBounds(area.removeFromLeft(200).reduced(margin));
}
