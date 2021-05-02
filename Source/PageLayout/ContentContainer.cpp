#include "ContentContainer.h"

ContentContainer::ContentContainer(ProjectState &ps)
: engine(JUCE_APPLICATION_NAME_STRING),
  mainHeader(engine, ps),
  pages(juce::TabbedButtonBar::Orientation::TabsAtTop),
  figures(engine, ps),
  particlesPage(engine, ps)
{
    addAndMakeVisible(&mainHeader);
    addAndMakeVisible(&pages);
    pages.addTab("Figures",
                 findColour(juce::ResizableWindow::backgroundColourId),
                 &figures,
                 true);
    pages.addTab("Particles",
                 findColour(juce::ResizableWindow::backgroundColourId),
                 &particlesPage,
                 true);
}

ContentContainer::~ContentContainer()
{}

void ContentContainer::paint(juce::Graphics &g)
{}

void ContentContainer::resized()
{
    auto area = getLocalBounds();
    mainHeader.setBounds(area.removeFromTop(30));
    pages.setBounds(area);
}
