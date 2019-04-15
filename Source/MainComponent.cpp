#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : engine(ProjectInfo::projectName),
                                 particleSelector(engine),
                                 sequencer(engine)
{
    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] { showAudioDeviceSettings(engine); };

    addAndMakeVisible(&particleSelector);
    addAndMakeVisible(&sequencer);

    // Set the size of the component after you add any child components
    setSize(600, 800);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::resized()
{
    settingsButton.setBounds(10, 10, getWidth() - 20, 20);
    particleSelector.setBounds(0, 40, getWidth(), 360);
    sequencer.setBounds(0, 400, getWidth(), 400);
}

void MainComponent::showAudioDeviceSettings(te::Engine &engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    o.content.setOwned(new AudioDeviceSelectorComponent(engine.getDeviceManager().deviceManager,
                                                        0, 512, 1, 512, false, false, true, true));
    o.content->setSize(400, 600);
    o.launchAsync();
}
