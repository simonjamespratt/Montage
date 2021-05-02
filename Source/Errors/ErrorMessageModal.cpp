#include "ErrorMessageModal.h"

ErrorMessageModal::ErrorMessageModal(juce::String message)
{
    juce::DialogWindow::LaunchOptions options;

    juce::Label *label = new juce::Label();
    label->setText(message, juce::dontSendNotification);
    label->setColour(juce::Label::textColourId, juce::Colours::red);
    options.content.setOwned(label);
    options.content->setSize(400, 200);

    options.dialogTitle = "Error message";
    options.resizable = false;

    options.launchAsync();
}
