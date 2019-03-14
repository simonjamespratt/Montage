#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : source()
{
    addAndMakeVisible(&source);
    // Set the size of the component after you add any child components
    setSize(600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::resized()
{
    source.setBounds(0, 0, getWidth(), getHeight());
}
