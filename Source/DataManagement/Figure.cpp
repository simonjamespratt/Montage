#include "Figure.h"

#include "CollectionHelper.h"
#include "Identifiers.h"
#include "StateChecker.h"

#include <algorithm>
#include <vector>

Figure::Figure(const juce::ValueTree &v) : state(v)
{
    StateChecker::checkTypeIsValid(state, IDs::FIGURE);
    std::vector<juce::Identifier> compulsoryProps {IDs::id};
    StateChecker::checkPropsAreValid(state, compulsoryProps);
    StateChecker::checkChildTypesAreValid(state, IDs::EVENT);
}

int Figure::getId()
{
    return state[IDs::id];
}

std::vector<Event> Figure::getEvents()
{
    std::vector<Event> events;

    for(int i = 0; i < state.getNumChildren(); i++) {
        auto eventState = state.getChild(i);
        events.push_back(Event(eventState));
    }

    return events;
}

Event Figure::createEvent(double onset, int particleId)
{
    int newId = CollectionHelper::createIdForNewChild(state);

    juce::ValueTree newEventState(IDs::EVENT);
    newEventState.setProperty(IDs::id, newId, nullptr);
    newEventState.setProperty(IDs::onset, onset, nullptr);
    newEventState.setProperty(IDs::particle_id, particleId, nullptr);
    state.appendChild(newEventState, nullptr);

    return Event(newEventState);
}
