#include "FigureProcessor.h"

FigureProcessor::FigureProcessor()
{}

Figure FigureProcessor::composeFigure(
    int numOfParticles,
    aleatoric::DurationsProducer &durationsProducer,
    aleatoric::CollectionsProducer<Particle> &collectionsProducer,
    ProjectState &projectState)
{
    Figure newFigure;
    auto figureList = projectState.getFigureList();
    figureList.addObject(newFigure);

    auto eventList = projectState.getEventList(newFigure);

    double onsetCount = 0;

    for(int i = 0; i < numOfParticles; i++) {
        auto selectedParticle = collectionsProducer.getItem();
        Event event(newFigure, selectedParticle, onsetCount);
        eventList.addObject(event);
        onsetCount += durationsProducer.getDuration();
    }

    newFigure.setIsGenerated(true);

    return newFigure;
}
