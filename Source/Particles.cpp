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
Particles::Particles(te::Engine &eng, ValueTree &as) : engine(eng), appState(as)
{
    addAndMakeVisible(&addParticleButton);
    addParticleButton.setButtonText("Add Particle");
    addParticleButton.onClick = [this] { addParticle(); };
}

Particles::~Particles()
{
}

void Particles::paint(Graphics &g)
{
}

void Particles::resized()
{
    addParticleButton.setBounds(0, 0, getWidth(), 20);

    int count = 30; //starting height to start laying out dynamically created ParticleSelectors

    for (int i = 0; i < particles.size(); i++)
    {
        particles[i]->setBounds(0, count, getWidth(), 360);
        count += 370;
    }
}

void Particles::changeListenerCallback(ChangeBroadcaster* source)
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
}

void Particles::addParticle()
{
    particles.emplace_back(std::make_unique<ParticleSelector>(engine, appState));
    addListeners();
    refreshView();
}

void Particles::refreshView()
{
    for (int i = 0; i < particles.size(); i++)
    {
        addAndMakeVisible(*particles[i]);
    }

    resized();
}

void Particles::addListeners()
{
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i]->addChangeListener(this);
    }

}
