#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ErrorMessageModal {
  public:
    ErrorMessageModal(juce::String message);

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ErrorMessageModal)
};
