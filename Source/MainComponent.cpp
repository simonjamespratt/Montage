#include "MainComponent.h"

#include "Identifiers.h"

MainComponent::MainComponent()
: projectInitialiser(projectState), contentContainer(projectState)
{
    addChildComponent(projectInitialiser);
    addChildComponent(contentContainer);
    auto s = projectState.getStatus();
    projectInitialiser.setVisible(!s.hasProjectDirectory);
    contentContainer.setVisible(s.hasProjectDirectory);

    projectState.onStatusChanged = [this](auto status, auto action) {
        projectInitialiser.setVisible(!status.hasProjectDirectory);
        contentContainer.setVisible(status.hasProjectDirectory);
    };

    // Set the size of the component after you add any child components
    auto screenSize = juce::Desktop::getInstance()
                          .getDisplays()
                          .getPrimaryDisplay()
                          ->userArea;
    setSize(screenSize.getWidth(), screenSize.getHeight());
}

MainComponent::~MainComponent()
{}

void MainComponent::resized()
{
    projectInitialiser.setBounds(getLocalBounds());
    contentContainer.setBounds(getLocalBounds());
}
