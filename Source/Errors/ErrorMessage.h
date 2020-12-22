#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class ErrorMessage : public juce::Component {
  public:
    ErrorMessage(juce::String messageReceived);
    ~ErrorMessage();

    void paint(juce::Graphics &) override;

  private:
    juce::String message;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ErrorMessage)
};
