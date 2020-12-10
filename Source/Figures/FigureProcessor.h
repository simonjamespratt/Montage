#pragma once
#include "Figure.h"
#include "FigureCollection.h"
#include "Particle.h"

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
                  FigureCollection &figureCollection);
};
