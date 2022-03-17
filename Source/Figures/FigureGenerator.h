#pragma once

#include "DurationProtocolParams.h"
#include "DurationProtocolSelector.h"
#include "Figure.h"
#include "NumberProtocolSelector.h"
#include "NumericValueEditorWithLabel.h"
#include "ProjectState.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class StageIndicator : public juce::Component {
  public:
    StageIndicator();
    void setStage(int stage);
    void paint(juce::Graphics &g) override;

  private:
    int stageIndex {-1};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StageIndicator)
};

class StageDescription : public juce::Component {
  public:
    StageDescription(juce::String titleText, juce::String descriptionText);
    void resized() override;

  private:
    juce::Label title;
    juce::TextEditor description;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StageDescription)
};

// fwd declaration
class FigureGenerator;

class FigGenStage : public juce::Component {
  public:
    FigGenStage(FigureGenerator &fg);
    ~FigGenStage();
    virtual void nextStage() = 0;
    virtual void previousStage() = 0;

  protected:
    FigureGenerator &figGen;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigGenStage)
};

class DurationProtocolStage : public FigGenStage {
  public:
    DurationProtocolStage(FigureGenerator &fg);
    ~DurationProtocolStage();
    void nextStage() override;
    void previousStage() override;
    void resized() override;

  private:
    StageDescription stageDescription {
        "1: Create the durations",
        "Creates a list of durations that can be chosen from by the "
        "generator when it makes an event."};

    DurationProtocolParams durationProtocolParams;
    DurationProtocolSelector durationProtocolSelector;
};

class DurationSelectionStage : public FigGenStage {
  public:
    DurationSelectionStage(FigureGenerator &fg);
    void nextStage() override;
    void previousStage() override;
    void resized() override;

  private:
    StageDescription stageDescription {
        "2: Choose how to select durations",
        "Choose a strategy that the generator uses for selecting which "
        "duration to choose from the durations list when making an event."};

    NumberProtocolSelector durationNumberProtocolSelector;
};

class ParticleSelectionStage : public FigGenStage {
  public:
    ParticleSelectionStage(FigureGenerator &fg);
    void nextStage() override;
    void previousStage() override;
    void resized() override;

  private:
    StageDescription stageDescription {
        "3: Choose how to select particles",
        "Choose a strategy for the generator to use for selecting which "
        "particle to place at each onset when making an event."};

    std::unique_ptr<NumberProtocolSelector> particleProtocolSelector;
};

class NoOfEventsStage : public FigGenStage {
  public:
    NoOfEventsStage(FigureGenerator &fg);
    ~NoOfEventsStage();
    void nextStage() override;
    void previousStage() override;
    void resized() override;

  private:
    StageDescription stageDescription {
        "4: Choose how many events to make",
        "Choose how many events the figure should have."};

    int noOfEvents {0};
    NumericValueEditorWithLabel noOfEventsInput;
};

class FigureGenerator : public juce::Component {
  public:
    FigureGenerator(ProjectState &ps);
    void resized() override;

    std::shared_ptr<aleatoric::DurationsProducer> durationsProducer;
    std::shared_ptr<aleatoric::CollectionsProducer<Particle>> particlesProducer;
    ProjectState projectState;

    StageIndicator stageIndicator;
    std::unique_ptr<FigGenStage> figGenStage;
    juce::TextButton nextBtn {"Next"};
    juce::TextButton previousBtn {"Previous"};
    juce::Label errorMessage;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FigureGenerator)
};
