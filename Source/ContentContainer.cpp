/*
  ==============================================================================

    ContentContainer.cpp
    Created: 29 Apr 2019 8:01:36pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ContentContainer.h"

//==============================================================================
ContentContainer::ContentContainer(ValueTree &as) : appState(as),
                                                    engine(ProjectInfo::projectName),
                                                    mainHeader(engine),
                                                    pages(TabbedButtonBar::Orientation::TabsAtTop),
                                                    figures(engine),
                                                    particlesPage(appState, engine)

{
    addAndMakeVisible(&mainHeader);
    addAndMakeVisible(&pages);
    pages.addTab("Figures", findColour(ResizableWindow::backgroundColourId), &figures, true);
    pages.addTab("Particles", findColour(ResizableWindow::backgroundColourId), &particlesPage, true);
}

ContentContainer::~ContentContainer()
{
}

void ContentContainer::paint(Graphics &g) {}

void ContentContainer::resized()
{
    auto area = getLocalBounds();
    mainHeader.setBounds(area.removeFromTop(100));
    pages.setBounds(area);
}
