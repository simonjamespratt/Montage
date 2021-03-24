#include "ContentContainer.h"

ContentContainer::ContentContainer(juce::ValueTree &as)
: appState(as),
  engine(JUCE_APPLICATION_NAME_STRING),
  mainHeader(engine),
  pages(juce::TabbedButtonBar::Orientation::TabsAtTop),
  figures(engine, appState),
  particlesPage(appState, engine)

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
