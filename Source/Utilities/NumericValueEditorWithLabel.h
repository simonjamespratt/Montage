#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

struct NumericValueEditorWithLabel : public juce::Component {
    NumericValueEditorWithLabel(int &editableValue,
                                juce::String labelText,
                                int labelSpace = 100);
    void resized() override;
    std::function<void()> onChange;
    void update();

  private:
    int &value;
    juce::TextEditor input;
    juce::Label label;
    int labelGutter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumericValueEditorWithLabel)
};
