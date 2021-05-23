#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// TODO: GENERAL-UI: This may well be a candidate for consolidating with Double
// and NumericValueEditorWithLabel
struct StringEditorWithLabel : public juce::Component {
    StringEditorWithLabel(juce::String &editableValue,
                          juce::String labelText,
                          int labelSpace = 100);
    void resized() override;

  private:
    juce::String &value;
    juce::TextEditor input;
    juce::Label label;
    int labelGutter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringEditorWithLabel)
};
