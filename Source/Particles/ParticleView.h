#pragma once
#include "Particle.h"
#include "TracktionThumbnail.h"

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace te = tracktion_engine;

class ParticleThumbnailOverlay : public juce::Component {
  public:
    ParticleThumbnailOverlay(const Particle &p);
    void paint(juce::Graphics &g) override;

  private:
    Particle particle;
};

class ParticleView : public juce::Component {
  public:
    ParticleView(const Particle &p, te::Engine &eng);
    void resized() override;
    void paint(juce::Graphics &g) override;

    std::function<void(Particle p)> onRemove;
    std::function<void(Particle p)> onEdit;

    void allowInteraction(bool isAllowed);

  private:
    Particle particle;

    juce::Label name;
    juce::Label sourceFileName;
    juce::Label start;
    juce::Label end;
    TracktionThumbnail thumbnail;
    ParticleThumbnailOverlay overlay;
    juce::TextButton removeButton;
    juce::TextButton editButton;

    void refreshView();
};
