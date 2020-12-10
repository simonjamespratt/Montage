#include "FigureProcessor.h"

FigureProcessor::FigureProcessor()
{}

Figure FigureProcessor::composeFigure(
    int numOfParticles,
    aleatoric::DurationsProducer &durationsProducer,
    aleatoric::CollectionsProducer<Particle> &collectionsProducer,
    FigureCollection &figureCollection)
{
    auto newFigure = figureCollection.createFigure();
    double onsetCount = 0;

    for(int i = 0; i < numOfParticles; i++) {
        auto selectedParticle = collectionsProducer.getItem();
        newFigure.createEvent(onsetCount, selectedParticle.getId());
        onsetCount += durationsProducer.getDuration();
    }

    return newFigure;
}
