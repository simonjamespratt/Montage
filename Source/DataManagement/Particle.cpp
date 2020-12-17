#include "Particle.h"

#include "StateChecker.h"

#include <stdexcept>
#include <vector>

Particle::Particle(const juce::ValueTree &v) : state(v)
{
    StateChecker::checkTypeIsValid(state, IDs::PARTICLE);

    std::vector<juce::Identifier> compulsoryProps {IDs::id,
                                                   IDs::source_id,
                                                   IDs::start,
                                                   IDs::end};

    StateChecker::checkPropsAreValid(state, compulsoryProps);
}

int Particle::getId() const
{
    return state[IDs::id];
}

int Particle::getSourceId() const
{
    return state[IDs::source_id];
}

double Particle::getStart() const
{
    return state[IDs::start];
}

double Particle::getEnd() const
{
    return state[IDs::end];
}
