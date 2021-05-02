#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

class DecisionModalContent : public juce::Component {
  public:
    DecisionModalContent(juce::String message,
                         std::function<void(bool decision)> onDecisionMade);

    void resized() override;

  private:
    juce::Label m_message;
    juce::TextButton confirmButton;
    juce::TextButton cancelButton;

    void closeModal();
};

class DecisionModal {
  public:
    DecisionModal(juce::String message,
                  std::function<void(bool decision)> onDecisionMade);
};
