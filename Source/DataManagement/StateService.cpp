#include "StateService.h"

#include "ErrorTypes.h"

#include <algorithm>
#include <stdexcept>

void StateService::checkTypeIsValid(const juce::ValueTree &state,
                                    juce::Identifier identifier)
{
    if(!state.hasType(identifier)) {
        throw ValueTreeInvalidType(identifier.toString().toStdString());
    }
}

void StateService::checkPropsAreValid(
    const juce::ValueTree &state,
    std::vector<juce::Identifier> compulsoryProps,
    std::vector<juce::Identifier> optionalProps)
{
    auto valueTreeType = state.getType().toString().toStdString();

    for(auto &&prop : compulsoryProps) {
        if(!state.hasProperty(prop)) {
            auto propName = prop.toString().toStdString();
            throw ValueTreeCompulsoryPropertyMissing(valueTreeType, propName);
        }
    }

    std::vector<juce::Identifier> allValidProps(compulsoryProps);
    allValidProps.insert(allValidProps.end(),
                         optionalProps.begin(),
                         optionalProps.end());

    for(int i = 0; i < state.getNumProperties(); i++) {
        auto prop = state.getPropertyName(i);
        auto propIsInvalid = std::find(std::begin(allValidProps),
                                       std::end(allValidProps),
                                       prop) == std::end(allValidProps);
        if(propIsInvalid) {
            auto propName = prop.toString().toStdString();
            throw ValueTreeUnexpectedPropertyReceived(valueTreeType, propName);
        }
    }
}

juce::ValueTree
StateService::getSourceStateForObject(const juce::ValueTree &objectState,
                                      const juce::ValueTree &stateToSearch)
{
    return getState(objectState, IDs::SOURCE, IDs::source_id, stateToSearch);
}

juce::ValueTree
StateService::getParticleStateForObject(const juce::ValueTree &objectState,
                                        const juce::ValueTree &stateToSearch)
{
    return getState(objectState,
                    IDs::PARTICLE,
                    IDs::particle_id,
                    stateToSearch);
}

juce::ValueTree
StateService::getFigureStateForObject(const juce::ValueTree &objectState,
                                      const juce::ValueTree &stateToSearch)
{
    return getState(objectState, IDs::FIGURE, IDs::figure_id, stateToSearch);
}

// Private methods
juce::ValueTree StateService::getState(const juce::ValueTree &objState,
                                       juce::Identifier typeToFind,
                                       juce::Identifier idTypeToFind,
                                       const juce::ValueTree &stateToSearch)
{
    jassert(objState.hasProperty(idTypeToFind));
    auto foundState =
        stateToSearch.getChildWithProperty(IDs::id, objState[idTypeToFind]);
    jassert(foundState.isValid() && foundState.hasType(typeToFind));
    return foundState;
}
