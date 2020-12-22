#include "FigureGenerator.h"

#include "DurationProtocolParams.h"
#include "FigureCollection.h"
#include "FigureProcessor.h"

#include <stdexcept>

FigureGenerator::FigureGenerator(juce::ValueTree as) : appState(as)
{
    // TODO: DATA-MANAGEMENT: when proper data handling is in place this will
    // need to be addressed
    appState.addListener(this);

    addAndMakeVisible(&blockedMessage);
    blockedMessage.setText("Not enough particles to generate a figure",
                           juce::dontSendNotification);

    globalSettingsHeading.setText("Global settings",
                                  juce::dontSendNotification);
    addChildComponent(&globalSettingsHeading);
    globalSettingsHeading.setFont(juce::Font(20.0f, juce::Font::bold));

    numEventsInput.setText("0", juce::dontSendNotification);
    addChildComponent(&numEventsInput);
    numEventsInput.setInputRestrictions(0, "0123456789");
    numEventsInput.setJustification(juce::Justification::centredLeft);

    numEventsLabel.setText("Number of events: ", juce::dontSendNotification);
    addChildComponent(&numEventsLabel);

    generateButton.setButtonText("Generate");
    addAndMakeVisible(&generateButton);
}

FigureGenerator::~FigureGenerator()
{}

void FigureGenerator::paint(juce::Graphics &g)
{}

void FigureGenerator::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    auto colWidth = area.getWidth() / 4;
    auto globalSettingsArea = area.removeFromLeft(colWidth);
    auto particleSelectionArea = area.removeFromLeft(colWidth);
    auto onsetSelectionArea = area;

    blockedMessage.setBounds(area);

    globalSettingsHeading.setBounds(globalSettingsArea.removeFromTop(50));

    auto numEventsArea = globalSettingsArea.removeFromTop(45);
    auto numEventsColWidth = numEventsArea.getWidth() / 3;
    numEventsLabel.setBounds(
        numEventsArea.removeFromLeft(numEventsColWidth * 2).reduced(margin));
    numEventsInput.setBounds(
        numEventsArea.removeFromRight(numEventsColWidth).reduced(margin));

    auto generateButtonArea =
        globalSettingsArea.removeFromTop(50).reduced(margin);
    generateButton.setBounds(generateButtonArea);

    if(figureParticleSelection != nullptr) {
        figureParticleSelection->setBounds(particleSelectionArea);
    }

    if(figureOnsetSelection != nullptr) {
        figureOnsetSelection->setBounds(onsetSelectionArea);
    }
}

Figure FigureGenerator::generateFigure()
{
    jassert(particleProducer != nullptr);

    auto figureCollectionState = appState.getChildWithName(IDs::FIGURES);
    FigureCollection figureCollection(figureCollectionState);

    auto numOfEventsToMake = numEventsInput.getText().getIntValue();

    if(numOfEventsToMake == 0) {
        // NB: this should really be a subclassed exception specific to number
        // of events specified when generating a figure. This will do for now
        // though
        throw std::runtime_error(
            "Invalid number of events specified during figure generation");
    }

    FigureProcessor processor;
    return processor.composeFigure(numOfEventsToMake,
                                   *onsetProducer,
                                   *particleProducer,
                                   figureCollection);
}

void FigureGenerator::valueTreeChildAdded(juce::ValueTree &parent,
                                          juce::ValueTree &childAdded)
{
    auto childType = childAdded.getType();

    if(childType == IDs::PARTICLES) {
        // when the particles sub-tree is added, create the particles collection
        particleCollection = std::make_unique<ParticleCollection>(childAdded);
    }

    if(childType == IDs::PARTICLE) {
        jassert(particleCollection != nullptr);

        auto particles = particleCollection->getParticles();

        if(particles.size() > 1) {
            if(particleProducer == nullptr) {
                particleProducer =
                    std::make_shared<aleatoric::CollectionsProducer<Particle>>(
                        particles,
                        aleatoric::NumberProtocol::create(
                            aleatoric::NumberProtocol::Type::basic));
            } else {
                particleProducer->setSource(particles);
            }

            if(figureParticleSelection == nullptr) {
                figureParticleSelection =
                    std::make_unique<FigureParticleSelection>(particleProducer);
                addAndMakeVisible(*figureParticleSelection);
            } else {
                figureParticleSelection->resetParams();
            }

            if(onsetProducer == nullptr) {
                onsetProducer = std::make_shared<aleatoric::DurationsProducer>(
                    aleatoric::DurationProtocol::createPrescribed(
                        std::vector<int> {1000, 2000}),
                    aleatoric::NumberProtocol::create(
                        aleatoric::NumberProtocol::Type::basic));
            }

            if(figureOnsetSelection == nullptr) {
                figureOnsetSelection = std::make_unique<FigureOnsetSelection>(
                    onsetProducer,
                    DurationProtocolParams(
                        DurationProtocolController::Type::prescribed));
                addAndMakeVisible(*figureOnsetSelection);
            }

            blockedMessage.setVisible(false);
            globalSettingsHeading.setVisible(true);
            numEventsInput.setVisible(true);
            numEventsLabel.setVisible(true);

            resized();
        }
    }
}

void FigureGenerator::valueTreeChildRemoved(juce::ValueTree &parent,
                                            juce::ValueTree &childRemoved,
                                            int index)
{
    auto childType = childRemoved.getType();

    if(childType == IDs::PARTICLE) {
        jassert(particleCollection != nullptr);

        auto particles = particleCollection->getParticles();

        if(particles.size() < 2) {
            particleProducer = nullptr;
            figureParticleSelection = nullptr;
            onsetProducer = nullptr;
            figureOnsetSelection = nullptr;

            blockedMessage.setVisible(true);
            globalSettingsHeading.setVisible(false);
            numEventsInput.setVisible(false);
            numEventsLabel.setVisible(false);

            resized();
        } else {
            particleProducer->setSource(particles);
            figureParticleSelection->resetParams();
        }
    }
}
