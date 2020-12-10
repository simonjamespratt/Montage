#include "Event.h"

#include "Identifiers.h"
#include "StateChecker.h"

#include <stdexcept>
#include <vector>

Event::Event(const juce::ValueTree &v) : state(v)
{
    StateChecker::checkTypeIsValid(state, IDs::EVENT);

    std::vector<juce::Identifier> compulsoryProps {IDs::id,
                                                   IDs::onset,
                                                   IDs::particle_id};

    StateChecker::checkPropsAreValid(state, compulsoryProps);
}

int Event::getId()
{
    return state[IDs::id];
}

double Event::getOnset()
{
    return state[IDs::onset];
}

int Event::getParticleId()
{
    return state[IDs::particle_id];
}
