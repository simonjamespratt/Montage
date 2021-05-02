#pragma once

#include "Icons.h"
#include "ParticleEditor.h"
#include "ParticleView.h"
#include "ProjectState.h"
#include "SourceSelector.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <vector>

namespace te = tracktion_engine;

class ParticlesManager : public juce::Component {
  public:
    ParticlesManager(const ProjectState &ps, te::Engine &eng);

    void resized() override;

  private:
    ProjectState projectState;
    ParticleList particleList;
    te::Engine &engine;
    std::vector<std::unique_ptr<ParticleView>> particleViews;
    juce::Component particlesContainer;
    juce::Viewport particlesViewport;

    juce::Label heading;
    Icons icons;
    juce::DrawablePath crossIcon;
    juce::DrawableButton addParticleButton;
    SourceSelector sourceSelector;
    std::unique_ptr<ParticleEditor> particleEditor;

    void refreshView();
    void removeParticle(Particle p);
    void openEditor(Particle p);
    void closeEditor();
    void allowInteractionInViews(bool isAllowed);
};
