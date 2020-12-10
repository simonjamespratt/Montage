#include "StateHelpers.h"

#include "Identifiers.h"

juce::ValueTree StateHelpers::getParticleState(int id)
{
    juce::ValueTree particle(IDs::PARTICLE);
    particle.setProperty(IDs::id, id, nullptr);
    particle.setProperty(IDs::source_id, 1000, nullptr);
    particle.setProperty(IDs::start, 100, nullptr);
    particle.setProperty(IDs::end, 200, nullptr);

    return particle;
}

juce::ValueTree StateHelpers::getFigureState(int id)
{
    juce::ValueTree figure(IDs::FIGURE);
    figure.setProperty(IDs::id, id, nullptr);
    return figure;
}

juce::ValueTree StateHelpers::getEventState(int id)
{
    juce::ValueTree event(IDs::EVENT);
    event.setProperty(IDs::id, id, nullptr);
    event.setProperty(IDs::onset, 100, nullptr);
    event.setProperty(IDs::particle_id, 1000, nullptr);
    return event;
}
