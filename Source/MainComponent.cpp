#include "MainComponent.h"

#include "Identifiers.h"

// constructor
MainComponent::MainComponent() : contentContainer(projectState)
{
    if(PRE_POPULATE_DUMMY_DATA) {
        auto filepath = juce::String(ASSETS_DIR) + "/presets/preset1.xml";
        juce::File file(filepath);
        projectState.load(file);
    }

    auto screenSize =
        juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    addAndMakeVisible(&contentContainer);

    // Set the size of the component after you add any child components
    setSize(screenSize.getWidth(), screenSize.getHeight());
}

MainComponent::~MainComponent()
{}

void MainComponent::resized()
{
    contentContainer.setBounds(getLocalBounds());
}
