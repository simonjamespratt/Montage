#pragma once

#include "Figure.h"
#include "FigureOnsetSelection.h"
#include "FigureParticleSelection.h"
#include "Identifiers.h"
#include "ProjectState.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class FigureGenerator : public juce::Component {
  public:
    FigureGenerator(ProjectState &ps);

    void resized() override;

    std::function<void(Figure f)> onFigureGenerated;

  private:
    ProjectState projectState;
    ParticleList particleList;

    std::shared_ptr<aleatoric::CollectionsProducer<Particle>> particleProducer;
    std::unique_ptr<FigureParticleSelection> figureParticleSelection;

    std::shared_ptr<aleatoric::DurationsProducer> onsetProducer;
    std::unique_ptr<FigureOnsetSelection> figureOnsetSelection;

    juce::Label blockedMessage;
    juce::Label globalSettingsHeading;
    juce::TextEditor numEventsInput;
    juce::Label numEventsLabel;
    juce::TextButton generateButton;

    void onParticleAdded();
    void onParticleRemoved();
    void generateFigure();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureGenerator)
};
