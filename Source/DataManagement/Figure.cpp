#include "Figure.h"

#include "ErrorTypes.h"
#include "Identifiers.h"
#include "StateService.h"

Figure::Figure()
{
    state = juce::ValueTree(IDs::FIGURE);
    state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    state.setProperty(IDs::name, juce::String("untitled"), nullptr);
    state.setProperty(IDs::is_generated, false, nullptr);

    state.addListener(this);
}

Figure::Figure(const juce::ValueTree &v) : state(v)
{
    StateService::checkTypeIsValid(state, IDs::FIGURE);
    std::vector<juce::Identifier> compulsoryProps {IDs::id,
                                                   IDs::name,
                                                   IDs::is_generated};
    StateService::checkPropsAreValid(state, compulsoryProps);

    state.addListener(this);

    juce::String name = state[IDs::name];

    if(juce::String(name).isEmpty()) {
        throw ObjectNameInvalid("Figure");
    }
}

Figure::~Figure()
{
    state.removeListener(this);
}

juce::Uuid Figure::getId() const
{
    return juce::Uuid(state[IDs::id]);
}

juce::String Figure::getName() const
{
    return state[IDs::name];
}

void Figure::setName(juce::String newName)
{
    if(newName.isEmpty()) {
        throw ObjectNameInvalid("Figure");
    }

    state.setProperty(IDs::name, newName, nullptr);
}

bool Figure::getIsGenerated() const
{
    return state[IDs::is_generated];
}

void Figure::setIsGenerated(bool isGenerated)
{
    state.setProperty(IDs::is_generated, isGenerated, nullptr);
}

juce::ValueTree Figure::getState() const
{
    return state;
}

// VT listener
void Figure::valueTreePropertyChanged(juce::ValueTree &vt,
                                      const juce::Identifier &property)
{
    if(vt == state && onUpdated) {
        onUpdated(property);
    }
}
