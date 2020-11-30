#include "MainComponent.h"

// constructor
MainComponent::MainComponent()
: appState(appStateIdentifier), contentContainer(appState)
{
    juce::ValueTree sources(sourcesIdentifier);
    juce::ValueTree particles(particlesIdentifier);
    juce::ValueTree figures(figuresIdentifier);
    appState.addChild(sources, -1, nullptr);
    appState.addChild(particles, -1, nullptr);
    appState.addChild(figures, -1, nullptr);

    auto screenSize =
        juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    addAndMakeVisible(&contentContainer);

    // Set the size of the component after you add any child components
    setSize(screenSize.getWidth(), screenSize.getHeight());
}

MainComponent::~MainComponent()
{}

//==============================================================================
void MainComponent::resized()
{
    contentContainer.setBounds(getLocalBounds());
}
