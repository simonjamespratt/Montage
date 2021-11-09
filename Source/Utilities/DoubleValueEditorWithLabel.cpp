#include "DoubleValueEditorWithLabel.h"

DoubleValueEditorWithLabel::DoubleValueEditorWithLabel(double &editableValue,
                                                       juce::String labelText,
                                                       int labelSpace)
: value(editableValue), labelGutter(labelSpace)
{
    input.setText(juce::String(value));
    input.setInputRestrictions(0, "0123456789.");
    input.setJustification(juce::Justification::centredLeft);
    input.onTextChange = [this] {
        value = input.getText().getDoubleValue();
        if(onChange) {
            onChange();
        }
    };
    input.onReturnKey = [this] {
        getTopLevelComponent()->grabKeyboardFocus();
    };

    label.setText(labelText + ": ", juce::dontSendNotification);
    label.attachToComponent(&input, true);

    addAndMakeVisible(&input);
    addAndMakeVisible(&label);
}

void DoubleValueEditorWithLabel::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    area.removeFromLeft(labelGutter); // make space for the label
    input.setBounds(area.removeFromLeft(100).reduced(margin));
}

void DoubleValueEditorWithLabel::update()
{
    input.setText(juce::String(value));
}
