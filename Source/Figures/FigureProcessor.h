#pragma once
#include "Figure.h"
#include "Particle.h"
#include "ProjectState.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <tuple>
#include <vector>

class FigureProcessor {
  public:
    FigureProcessor();

    Figure
    composeFigure(int numOfParticles,
                  aleatoric::DurationsProducer &durationsProducer,
                  aleatoric::CollectionsProducer<Particle> &collectionsProducer,
                  ProjectState &projectState);
};
