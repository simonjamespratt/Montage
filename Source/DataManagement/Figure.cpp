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
    StateService::checkPropsAreValid(state,
                                     compulsoryProps,
                                     {IDs::DURATION_SETTINGS});

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

std::unique_ptr<FigureCreationSettings> Figure::getCreationSettings() const
{
    auto durationsState = state.getChildWithName(IDs::DURATION_SETTINGS);

    std::vector<NumberProtocolSettings> numberSettings;

    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);

        if(child.hasType(IDs::NUMBER_SETTINGS)) {
            numberSettings.emplace_back(NumberProtocolSettings(child));
        }
    }

    if(durationsState.isValid() && numberSettings.size() == 2) {
        auto settings = std::make_unique<FigureCreationSettings>(
            DurationProtocolSettings(durationsState),
            numberSettings[0].context == CreationContext::duration_selection
                ? numberSettings[0]
                : numberSettings[1],
            numberSettings[0].context == CreationContext::particle_selection
                ? numberSettings[0]
                : numberSettings[1]);
        return settings;
    }

    return nullptr;
}

void Figure::setCreationSettings(const FigureCreationSettings &settings)
{
    std::vector<juce::ValueTree>
        toBeRemoved {}; // NB: do removal separately because removing changes
                        // the indices of the underlying data and the index in
                        // the loop will no longer refer to the correct child
    for(int i = 0; i < state.getNumChildren(); i++) {
        auto child = state.getChild(i);
        if(child.hasType(IDs::NUMBER_SETTINGS) ||
           child.hasType(IDs::DURATION_SETTINGS)) {
            toBeRemoved.emplace_back(child);
        }
    }
    for(auto &child : toBeRemoved) {
        state.removeChild(child, nullptr);
    }

    state.addChild(settings.durations.state, -1, nullptr);
    state.addChild(settings.durationSelection.state, -1, nullptr);
    state.addChild(settings.particleSelection.state, -1, nullptr);
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
