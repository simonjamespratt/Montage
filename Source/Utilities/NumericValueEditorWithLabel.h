#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

struct NumericValueEditorWithLabel : public juce::Component,
                                     private juce::TextEditor::Listener {
    NumericValueEditorWithLabel(int &editableValue,
                                juce::String labelText,
                                int labelSpace = 100);
    ~NumericValueEditorWithLabel();

    void resized() override;
    std::function<void()> onChange;
    std::function<void()> onFocusLost;
    void update();

  private:
    void textEditorFocusLost(juce::TextEditor &) override;

    int &value;
    juce::TextEditor input;
    juce::Label label;
    int labelGutter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumericValueEditorWithLabel)
};
