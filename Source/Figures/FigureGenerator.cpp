#include "FigureGenerator.h"

#include "DurationProtocolParams.h"
#include "ErrorMessageModal.h"
#include "FigureProcessor.h"

FigureGenerator::FigureGenerator(ProjectState &ps)
: projectState(ps), particleList(ps.getParticleList())
{
    particleList.onObjectAdded = [this](Particle) {
        onParticleAdded();
    };
    particleList.onObjectRemoved = [this](Particle) {
        onParticleRemoved();
    };

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
    generateButton.onClick = [this] {
        generateFigure();
    };
}

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

void FigureGenerator::generateFigure()
{
    jassert(particleProducer != nullptr);

    auto numOfEventsToMake = numEventsInput.getText().getIntValue();

    if(numOfEventsToMake == 0) {
        std::make_shared<ErrorMessageModal>(
            "The number of events specified for a "
            "figure must be greater than 0");
        return;
    }

    FigureProcessor processor;
    try {
        auto newFigure = processor.composeFigure(numOfEventsToMake,
                                                 *onsetProducer,
                                                 *particleProducer,
                                                 projectState);
        if(onFigureGenerated) {
            onFigureGenerated(newFigure);
        }
    } catch(const std::exception &e) {
        /*
        Catches:
        - Figure creation - no errors thrown
        - FigureList addition: ObjectAlreadyExists
        - Event creation: EventOnsetInvalid
        - EventList addition:
            - ObjectDependencyNotFound,
            - ObjectAlreadyExists,
            - EventFigureInvalidForEventList
        */
        std::make_shared<ErrorMessageModal>(juce::String(e.what()));
        return;
    }
}

// Private methods
void FigureGenerator::onParticleAdded()
{
    auto particles = particleList.getObjects();

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

void FigureGenerator::onParticleRemoved()
{
    auto particles = particleList.getObjects();

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
