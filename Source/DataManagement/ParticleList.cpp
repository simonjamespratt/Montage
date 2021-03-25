#include "ParticleList.h"

#include "ErrorTypes.h"
#include "StateService.h"

ParticleList::ParticleList(juce::ValueTree vt) : ObjectList<Particle>(vt)
{}

// Private methods
bool ParticleList::isSuitableType(const juce::ValueTree &vt) const
{
    return vt.hasType(IDs::PARTICLE);
}

Particle ParticleList::makeObject(const juce::ValueTree &vt) const
{
    return Particle(vt,
                    Source(StateService::getSourceStateForObject(vt, state)));
}

juce::ValueTree ParticleList::getObjectState(const Particle &particle) const
{
    return particle.getState();
}

void ParticleList::validateObjectAddition(const Particle &particle)
{
    auto sourceState =
        state.getChildWithProperty(IDs::id, particle.getSourceId().toString());

    if(!sourceState.isValid()) {
        throw ObjectDependencyNotFound("Particle", "Source");
    }
}

void ParticleList::validateObjectRemoval(const Particle &particle)
{
    auto particleState = particle.getState();

    auto childUsingSource =
        state.getChildWithProperty(IDs::particle_id, particleState[IDs::id]);

    if(childUsingSource.isValid()) {
        throw ObjectInUse();
    }
}
