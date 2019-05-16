/*
  ==============================================================================

    Particles.h
    Created: 17 Apr 2019 7:46:25pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./ParticleSelector.h"
#include "Icons.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Particles : public Component, public ChangeListener
{
public:
    Particles(te::Engine &eng, ValueTree &as);
    ~Particles();

    void paint(Graphics &g) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    te::Engine &engine;
    ValueTree &appState;
    Label heading;
    Icons icons;
    DrawablePath crossIcon;
    DrawableButton addParticleButton;
    std::vector<std::unique_ptr<ParticleSelector>> particles;
    int headerHeight;
    void addParticle();
    void recalculateSize();
    void refreshView();
    void addListeners();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Particles)
};
