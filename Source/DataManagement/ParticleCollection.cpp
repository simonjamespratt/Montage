#include "ParticleCollection.h"

#include "StateChecker.h"

#include <stdexcept>

ParticleCollection::ParticleCollection(const juce::ValueTree &v) : state(v)
{
    StateChecker::checkTypeIsValid(state, IDs::PARTICLES);
    StateChecker::checkChildTypesAreValid(state, IDs::PARTICLE);
}

const std::vector<Particle> ParticleCollection::getParticles()
{
    std::vector<Particle> particles;

    for(int i = 0; i < state.getNumChildren(); i++) {
        auto particleState = state.getChild(i);
        particles.push_back(Particle(particleState));
    }

    return particles;
}
