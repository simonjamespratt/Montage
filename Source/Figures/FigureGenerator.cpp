#include "FigureGenerator.h"

#include "FigureCollection.h"
#include "FigureProcessor.h"
#include "ParticleCollection.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>

FigureGenerator::FigureGenerator(juce::ValueTree &as) : appState(as)
{
    holdingMessage.setText("holding area for figure generation options.",
                           juce::dontSendNotification);
    addAndMakeVisible(&holdingMessage);
}

FigureGenerator::~FigureGenerator()
{}

void FigureGenerator::paint(juce::Graphics &g)
{}

void FigureGenerator::resized()
{
    auto area = getLocalBounds();
    holdingMessage.setBounds(area.removeFromTop(50));
}

Figure FigureGenerator::generateFigure()
{
    using namespace aleatoric;

    // NB: note that particles is NOT a member of the class and only gets
    // particles from appState at the last minute, just before using it. This is
    // because of the difference between a copy and a reference. appState is a
    // reference which will update, whereas a copy won't so if you try and take
    // a copy of particles from appState in the constructor, you will have an
    // outdated version copies as members work fine when you are using ValueTree
    // listeners because you can update the member (copy) when you get a
    // notification from the listener but all you are really doing is the same
    // as here - taking a copy from the reference to appState particles here
    // could be a reference but I can't work out how to initialize it properly
    auto particleCollectionState = appState.getChildWithName(IDs::PARTICLES);
    ParticleCollection particleCollection(particleCollectionState);

    auto figureCollectionState = appState.getChildWithName(IDs::FIGURES);
    FigureCollection figureCollection(figureCollectionState);

    // TODO: delete this when UI for selection of protocol etc. is in place
    int numOfEventsToMake = 8;
    std::vector<int> durations {1000, 2000};

    // Basic predictable results using prescribed durations and cycling of
    // durations and particles
    DurationsProducer durationsProducer(
        DurationProtocol::createPrescribed(durations),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    CollectionsProducer<Particle> collectionsProducer(
        particleCollection.getParticles(),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    FigureProcessor processor;
    return processor.composeFigure(numOfEventsToMake,
                                   durationsProducer,
                                   collectionsProducer,
                                   figureCollection);
}
