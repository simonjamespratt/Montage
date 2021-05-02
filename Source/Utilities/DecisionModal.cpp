#include "DecisionModal.h"

DecisionModalContent::DecisionModalContent(
    juce::String message, std::function<void(bool decision)> onDecisionMade)
{
    m_message.setText(message, juce::dontSendNotification);
    addAndMakeVisible(m_message);

    confirmButton.setButtonText("Yes");
    confirmButton.onClick = [=] {
        onDecisionMade(true);
        closeModal();
    };
    addAndMakeVisible(confirmButton);

    cancelButton.setButtonText("No");
    cancelButton.onClick = [=] {
        onDecisionMade(false);
        closeModal();
    };
    addAndMakeVisible(cancelButton);
}

void DecisionModalContent::resized()
{
    auto area = getLocalBounds();
    auto buttonsArea = area.removeFromBottom(45);

    confirmButton.setBounds(buttonsArea.removeFromLeft(100).reduced(10));
    cancelButton.setBounds(buttonsArea.removeFromRight(100).reduced(10));
    m_message.setBounds(area.reduced(10));
}

void DecisionModalContent::closeModal()
{
    if(juce::DialogWindow *dw =
           findParentComponentOfClass<juce::DialogWindow>()) {
        dw->exitModalState(0);
    }
}

DecisionModal::DecisionModal(juce::String message,
                             std::function<void(bool decision)> onDecisionMade)
{
    juce::DialogWindow::LaunchOptions options;

    options.content.setOwned(new DecisionModalContent(message, onDecisionMade));
    options.content->setSize(400, 200);
    options.dialogBackgroundColour = juce::Colours::darkgrey;
    options.dialogTitle = "Action required";
    options.resizable = false;

    options.launchAsync();
}
