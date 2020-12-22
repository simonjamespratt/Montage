#include "NumericCollectionEditor.h"

#include <iterator>

NumericItemEditor::NumericItemEditor(int &value,
                                     int index,
                                     std::function<void(int index)> onDelete)
: valueEditor(value, juce::String(index + 1), 25)
{
    deleteButton.setButtonText("Delete");
    deleteButton.onClick = [index, onDelete] {
        onDelete(index);
    };

    valueEditor.onChange = [this] {
        if(onChange) {
            onChange();
        }
    };

    addAndMakeVisible(&valueEditor);
    addAndMakeVisible(&deleteButton);
};

NumericItemEditor::NumericItemEditor(int &value, int index)
: valueEditor(value, juce::String(index + 1), 25)
{
    valueEditor.onChange = [this] {
        if(onChange) {
            onChange();
        }
    };

    addAndMakeVisible(&valueEditor);
}

void NumericItemEditor::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    valueEditor.setBounds(area.removeFromLeft(150));

    if(deleteButton.isVisible()) {
        deleteButton.setBounds(area.reduced(margin));
    }
}

// ==========================================================================

NumericCollectionEditor::NumericCollectionEditor(
    std::vector<int> &numericCollection, bool isEditOnly)
: collection(numericCollection), isInEditOnlyMode(isEditOnly)
{
    drawView();

    if(!isInEditOnlyMode) {
        addButton.setButtonText("Add value");
        addButton.onClick = [this] {
            onAdd();
        };
        addAndMakeVisible(&addButton);
    }
}

void NumericCollectionEditor::resized()
{
    auto margin = 10;
    int componentHeight = 45;

    auto totalHeight = getNumChildComponents() * componentHeight;
    setSize(getWidth(), totalHeight);

    auto area = getLocalBounds();

    for(auto &editor : editors) {
        editor->setBounds(area.removeFromTop(componentHeight));
    }

    if(addButton.isVisible()) {
        addButton.setBounds(
            area.removeFromTop(componentHeight).reduced(margin));
    }
}

void NumericCollectionEditor::redraw()
{
    drawView();
    resized();
}

// Private methods
void NumericCollectionEditor::drawView()
{
    editors.clear();
    for(auto it = begin(collection); it != end(collection); ++it) {
        int index = std::distance(collection.begin(), it);

        std::unique_ptr<NumericItemEditor> editor;

        if(isInEditOnlyMode) {
            editor = std::make_unique<NumericItemEditor>(*it, index);
        } else {
            editor = std::make_unique<NumericItemEditor>(
                *it,
                index,
                [this](int index) { onDelete(index); });
        }

        editor->onChange = [this] {
            if(onChange) {
                onChange();
            }
        };
        addAndMakeVisible(*editor);
        editors.emplace_back(std::move(editor));
    }
}

void NumericCollectionEditor::onDelete(int index)
{
    collection.erase(collection.begin() + index);
    drawView();
    resized();

    if(onChange) {
        onChange();
    }
}

void NumericCollectionEditor::onAdd()
{
    collection.emplace_back(1);
    drawView();
    resized();

    if(onChange) {
        onChange();
    }
}
