#pragma once

#include "Icons.h"
#include "ParticleSelector.h"

namespace te = tracktion_engine;

class Particles : public juce::Component, public juce::ChangeListener {
  public:
    Particles(te::Engine &eng, juce::ValueTree &as);
    ~Particles();

    void paint(juce::Graphics &g) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

  private:
    te::Engine &engine;
    juce::ValueTree &appState;
    juce::Label heading;
    Icons icons;
    juce::DrawablePath crossIcon;
    juce::DrawableButton addParticleButton;
    std::vector<std::unique_ptr<ParticleSelector>> particles;
    int headerHeight;
    void addParticle();
    void recalculateSize();
    void refreshView();
    void addListeners();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Particles)
};
