/*
  ==============================================================================

    Particles.cpp
    Created: 17 Apr 2019 7:46:25pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Particles.h"

//==============================================================================
Particles::Particles(te::Engine &eng, ValueTree &as) :  engine(eng),
                                                        appState(as),
                                                        crossIcon(icons.getIcon(Icons::IconType::Cross)),
                                                        addParticleButton("Add particle button", DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    heading.setText("Particles", dontSendNotification);
    heading.setFont(Font(24.0f, Font::bold));
    addAndMakeVisible(&heading);
    
    addParticleButton.setImages(&crossIcon);
    addAndMakeVisible(&addParticleButton);
    addParticleButton.onClick = [this] { addParticle(); };
    
    recalculateSize();
}

Particles::~Particles()
{
}

void Particles::paint(Graphics &g)
{
}

void Particles::resized()
{
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(headerHeight);
    FlexBox headerContainer;
    headerContainer.justifyContent = FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = FlexBox::AlignContent::center;
    headerContainer.items.add(FlexItem(heading).withHeight(24.0f).withWidth(100.0f).withMargin(FlexItem::Margin(5.0f)));
    headerContainer.items.add(FlexItem(addParticleButton).withHeight(24.0f).withWidth(24.0f).withMargin(FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);

    for (int i = 0; i < particles.size(); i++)
    {
        particles[i]->setBounds(area.removeFromTop(300));
    }
}

void Particles::changeListenerCallback(ChangeBroadcaster *source)
{
    // find out which particle did this
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        if ((*it)->readyToBeDeleted())
        {
            // delete the object and remove from array
            particles.erase(it--);
        }
    }

    refreshView();
}

void Particles::addParticle()
{
    particles.emplace_back(std::make_unique<ParticleSelector>(engine, appState));
    addListeners();
    refreshView();
}

void Particles::recalculateSize()
{
    headerHeight = 50;
    auto totalHeight = headerHeight;
    
    for (int i = 0; i < particles.size(); i++)
    {
        totalHeight += 300;
    }
    
    setSize(getWidth(), totalHeight);
}

void Particles::refreshView()
{
    for (int i = 0; i < particles.size(); i++)
    {
        addAndMakeVisible(*particles[i]);
    }

    recalculateSize();
}

void Particles::addListeners()
{
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i]->addChangeListener(this);
    }
}
