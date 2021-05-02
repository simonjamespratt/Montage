#include "Figure.h"

#include "Identifiers.h"
#include "StateService.h"

Figure::Figure()
{
    state = juce::ValueTree(IDs::FIGURE);
    state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
}

Figure::Figure(const juce::ValueTree &v) : state(v)
{
    StateService::checkTypeIsValid(state, IDs::FIGURE);
    std::vector<juce::Identifier> compulsoryProps {IDs::id};
    StateService::checkPropsAreValid(state, compulsoryProps);
}

juce::Uuid Figure::getId() const
{
    return juce::Uuid(state[IDs::id]);
}

juce::ValueTree Figure::getState() const
{
    return state;
}
