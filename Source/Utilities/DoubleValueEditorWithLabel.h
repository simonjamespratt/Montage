#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// TODO: GENERAL-UI: This and NumericValueEditorWithLabel are
// basically the same component. Didn't have time to work out how to make them
// work together. Will be something around templates, overloads and/or
// inheritance
struct DoubleValueEditorWithLabel : public juce::Component {
    DoubleValueEditorWithLabel(double &editableValue,
                               juce::String labelText,
                               int labelSpace = 100);
    void resized() override;
    std::function<void()> onChange;
    void update();

  private:
    double &value;
    juce::TextEditor input;
    juce::Label label;
    int labelGutter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DoubleValueEditorWithLabel)
};
