#include "Event.h"

#include "ErrorTypes.h"
#include "Identifiers.h"
#include "StateService.h"

#include <stdexcept>
#include <vector>

Event::Event(const juce::ValueTree &v, const Figure &f, const Particle &p)
: state(v), particle(p), figure(f)
{
    StateService::checkTypeIsValid(state, IDs::EVENT);

    std::vector<juce::Identifier> compulsoryProps {IDs::id,
                                                   IDs::onset,
                                                   IDs::particle_id,
                                                   IDs::figure_id};

    StateService::checkPropsAreValid(state, compulsoryProps);

    double onset = {state[IDs::onset]};
    juce::Uuid figureId(state[IDs::figure_id]);
    juce::Uuid particleId(state[IDs::particle_id]);

    if(figure.getId() != figureId) {
        throw ObjectDependencyInvalid("Event", "Figure");
    }

    if(particle.getId() != particleId) {
        throw ObjectDependencyInvalid("Event", "Particle");
    }

    validateOnset(onset);

    state.addListener(this);
}

Event::Event(const Figure &f, const Particle &p, double onset)
: particle(p), figure(f)
{
    validateOnset(onset);

    state = juce::ValueTree(IDs::EVENT);
    state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    state.setProperty(IDs::onset, onset, nullptr);
    state.setProperty(IDs::particle_id, particle.getId().toString(), nullptr);
    state.setProperty(IDs::figure_id, figure.getId().toString(), nullptr);

    state.addListener(this);
}

Event::~Event()
{
    state.removeListener(this);
}

juce::Uuid Event::getId() const
{
    return juce::Uuid(state[IDs::id]);
}

double Event::getOnset() const
{
    return state[IDs::onset];
}

void Event::setOnset(double newOnset)
{
    validateOnset(newOnset);
    state.setProperty(IDs::onset, newOnset, nullptr);
}

juce::Uuid Event::getParticleId() const
{
    return juce::Uuid(state[IDs::particle_id]);
}

Particle Event::getParticle() const
{
    return particle;
}

juce::Uuid Event::getFigureId() const
{
    return juce::Uuid(state[IDs::figure_id]);
}

Figure Event::getFigure() const
{
    return figure;
}

juce::ValueTree Event::getState() const
{
    return state;
}

// VT listeners
void Event::valueTreePropertyChanged(juce::ValueTree &vt,
                                     const juce::Identifier &property)
{
    if(vt == state && onUpdated) {
        onUpdated(property);
    }
}

// Private methods
void Event::validateOnset(double newOnset)
{
    if(newOnset < 0) {
        throw EventOnsetInvalid();
    }
}
