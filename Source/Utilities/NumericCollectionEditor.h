#pragma once

#include "NumericValueEditorWithLabel.h"

#include <functional>
#include <memory>
#include <vector>

struct NumericItemEditor : public juce::Component {
    NumericItemEditor(int &value,
                      int index,
                      std::function<void(int index)> onDelete);

    NumericItemEditor(int &value, int index);

    void resized() override;
    std::function<void()> onChange;

  private:
    NumericValueEditorWithLabel valueEditor;
    juce::TextButton deleteButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumericItemEditor)
};

struct NumericCollectionEditor : public juce::Component {
    NumericCollectionEditor(std::vector<int> &numericCollection,
                            bool isEditOnly = false);
    void resized() override;
    std::function<void()> onChange;
    void redraw();

  private:
    void drawView();
    void onAdd();
    void onDelete(int index);

    std::vector<int> &collection;

    // NB: have to use pointers because juce components have their copy
    // constructors deleted and both vector and list (despite what link below
    // says) require objects being stored to have copy constructors. See:
    // https://forum.juce.com/t/adding-components-to-std-vector-with-emplace-back/35193
    std::vector<std::unique_ptr<NumericItemEditor>> editors {};
    juce::TextButton addButton;
    bool isInEditOnlyMode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumericCollectionEditor)
};
