#include "MainComponent.h"

// constructor
MainComponent::MainComponent() : /*source(),*/
                                 sequencer()
{
    // addAndMakeVisible(&source);
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
    // source.setBounds(0, 0, getWidth(), 400);
    sequencer.setBounds(0, 400, getWidth(), 400);
}
