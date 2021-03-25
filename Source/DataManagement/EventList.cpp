#include "EventList.h"

#include "StateService.h"

#include <stdexcept>

EventList::EventList(juce::ValueTree vt) : ObjectList<Event>(vt)
{}

EventList::EventList(juce::ValueTree vt, const Figure &f)
: ObjectList<Event>(vt)
{
    figure = std::make_shared<Figure>(f);
}

// Private methods
bool EventList::isSuitableType(const juce::ValueTree &vt) const
{
    if(figure == nullptr) {
        return vt.hasType(IDs::EVENT);
    }

    return vt.hasType(IDs::EVENT) &&
           juce::Uuid(vt[IDs::figure_id]) == figure->getId();
}

Event EventList::makeObject(const juce::ValueTree &vt) const
{
    auto particleState = StateService::getParticleStateForObject(vt, state);
    Source source(StateService::getSourceStateForObject(particleState, state));
    Particle particle(particleState, source);
    Figure figure(StateService::getFigureStateForObject(vt, state));
    return Event(vt, figure, particle);
}

juce::ValueTree EventList::getObjectState(const Event &event) const
{
    return event.getState();
}

void EventList::validateObjectAddition(const Event &event)
{
    if(figure != nullptr && event.getFigureId() != figure->getId()) {
        throw EventFigureInvalidForEventList();
    }

    auto figureState =
        state.getChildWithProperty(IDs::id, event.getFigureId().toString());

    if(!figureState.isValid()) {
        throw ObjectDependencyNotFound("Event", "Figure");
    }

    auto particleState =
        state.getChildWithProperty(IDs::id, event.getParticleId().toString());

    if(!particleState.isValid()) {
        throw ObjectDependencyNotFound("Event", "Particle");
    }
}

void EventList::validateObjectRemoval(const Event &event)
{
    if(figure != nullptr && event.getFigureId() != figure->getId()) {
        throw EventFigureInvalidForEventList();
    }
}
