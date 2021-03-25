#include "ProjectState.h"

#include "Identifiers.h"

ProjectState::ProjectState(juce::ValueTree vt) : state(vt)
{
    jassert(state.hasType(IDs::PROJECT_STATE));
}

ParticleList ProjectState::getParticleList() const
{
    return ParticleList(state);
}

SourceList ProjectState::getSourceList() const
{
    return SourceList(state);
}

FigureList ProjectState::getFigureList() const
{
    return FigureList(state);
}

EventList ProjectState::getEventList() const
{
    return EventList(state);
}

EventList ProjectState::getEventList(const Figure &f) const
{
    return EventList(state, f);
}
