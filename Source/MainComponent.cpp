#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : appState(appStateIdentifier),
                                 engine(ProjectInfo::projectName),
                                 sourceManager(appState),
                                 particlesManifest(appState),
                                 //  particleSelector(engine),
                                 //  sequencer(engine),
                                 particlesContainer(engine, appState)
{
    ValueTree sources(sourcesIdentifier);
    ValueTree particles(particlesIdentifier);

    appState.addChild(sources, -1, nullptr);
    appState.addChild(particles, -1, nullptr);

    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] { showAudioDeviceSettings(engine); };

    // addAndMakeVisible(&particleSelector);
    // addAndMakeVisible(&sequencer);
    addAndMakeVisible(&sourceManager);
    addAndMakeVisible(&particlesManifest);
    addAndMakeVisible(&particlesContainer);

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
    // particleSelector.setBounds(0, 40, getWidth(), 360);
    // sequencer.setBounds(0, 400, getWidth(), 400);
    sourceManager.setBounds(10, 40, getWidth() - 20, 200);
    particlesManifest.setBounds(10, 250, getWidth() - 20, 100);
    particlesContainer.setBounds(10, 360, getWidth() - 20, 450);
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
