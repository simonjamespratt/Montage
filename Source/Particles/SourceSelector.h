#pragma once

#include "ProjectState.h"

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

class SourceSelector : public juce::Component {
  public:
    SourceSelector(SourceList sl);
    void resized() override;

    std::function<void(Source s)> onSourceChosen;
    std::function<void()> onCancel;

  private:
    SourceList sourceList;
    juce::Label helperText;
    juce::ComboBox sourceSelector;
    juce::TextButton confirmButton;
    juce::TextButton cancelButton;

    void configureSourceSelector();
};
