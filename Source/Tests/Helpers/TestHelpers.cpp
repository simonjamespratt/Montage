#include "TestHelpers.h"

#include "Identifiers.h"

// Particle ========================================================
juce::ValueTree StateHelpers::createParticleState()
{
    juce::ValueTree particle(IDs::PARTICLE);
    particle.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    particle.setProperty(IDs::source_id, juce::Uuid().toString(), nullptr);
    particle.setProperty(IDs::start, 0.0, nullptr);
    particle.setProperty(IDs::end, 1.0, nullptr);
    particle.setProperty(IDs::name, juce::String("name"), nullptr);

    return particle;
}

juce::ValueTree StateHelpers::createParticleState(juce::Uuid id,
                                                  juce::Uuid sourceId,
                                                  double start,
                                                  double end,
                                                  juce::String name)
{
    juce::ValueTree particle(IDs::PARTICLE);
    particle.setProperty(IDs::id, id.toString(), nullptr);
    particle.setProperty(IDs::source_id, sourceId.toString(), nullptr);
    particle.setProperty(IDs::start, start, nullptr);
    particle.setProperty(IDs::end, end, nullptr);
    particle.setProperty(IDs::name, name, nullptr);

    return particle;
}

// Source ==========================================================
juce::ValueTree StateHelpers::createSourceState()
{
    juce::ValueTree source(IDs::SOURCE);
    source.setProperty(IDs::id, juce::Uuid().toString(), nullptr);

    auto filepath = juce::String(ASSETS_DIR) + "/audio/whitenoise-2000ms.wav";
    source.setProperty(IDs::file_path, filepath, nullptr);
    return source;
}

juce::ValueTree StateHelpers::createSourceState(juce::Uuid id)
{
    juce::ValueTree source(IDs::SOURCE);
    source.setProperty(IDs::id, id.toString(), nullptr);

    auto filepath = juce::String(ASSETS_DIR) + "/audio/whitenoise-2000ms.wav";
    source.setProperty(IDs::file_path, filepath, nullptr);
    return source;
}

juce::ValueTree StateHelpers::createSourceState(juce::Uuid id,
                                                juce::String filePath)
{
    juce::ValueTree source(IDs::SOURCE);
    source.setProperty(IDs::id, id.toString(), nullptr);
    source.setProperty(IDs::file_path, filePath, nullptr);
    return source;
}

// Figure ==========================================================
juce::ValueTree StateHelpers::createFigureState()
{
    juce::ValueTree figure(IDs::FIGURE);
    figure.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    return figure;
}

juce::ValueTree StateHelpers::createFigureState(juce::Uuid id)
{
    juce::ValueTree figure(IDs::FIGURE);
    figure.setProperty(IDs::id, id.toString(), nullptr);
    return figure;
}

// Event ===========================================================
juce::ValueTree StateHelpers::createEventState()
{
    juce::ValueTree event(IDs::EVENT);
    event.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    event.setProperty(IDs::onset, 100, nullptr);
    event.setProperty(IDs::particle_id, juce::Uuid().toString(), nullptr);
    event.setProperty(IDs::figure_id, juce::Uuid().toString(), nullptr);
    return event;
}

juce::ValueTree StateHelpers::createEventState(juce::Uuid id)
{
    juce::ValueTree event(IDs::EVENT);
    event.setProperty(IDs::id, id.toString(), nullptr);
    event.setProperty(IDs::onset, 100, nullptr);
    event.setProperty(IDs::particle_id, juce::Uuid().toString(), nullptr);
    event.setProperty(IDs::figure_id, juce::Uuid().toString(), nullptr);
    return event;
}

juce::ValueTree StateHelpers::createEventState(juce::Uuid id,
                                               double onset,
                                               juce::Uuid particleId,
                                               juce::Uuid figureId)
{
    juce::ValueTree event(IDs::EVENT);
    event.setProperty(IDs::id, id.toString(), nullptr);
    event.setProperty(IDs::onset, onset, nullptr);
    event.setProperty(IDs::particle_id, particleId.toString(), nullptr);
    event.setProperty(IDs::figure_id, figureId.toString(), nullptr);
    return event;
}

// Project state ==================================================
juce::ValueTree StateHelpers::createProjectState(int noOfSources,
                                                 int noOfParticles,
                                                 int noOfFigures,
                                                 int noOfEvents)
{
    juce::ValueTree projectState(IDs::PROJECT_STATE);

    for(int i = 0; i < noOfSources; i++) {
        projectState.appendChild(createSourceState(), nullptr);
    }
    for(int i = 0; i < noOfParticles; i++) {
        projectState.appendChild(createParticleState(), nullptr);
    }
    for(int i = 0; i < noOfFigures; i++) {
        projectState.appendChild(createFigureState(), nullptr);
    }
    for(int i = 0; i < noOfEvents; i++) {
        projectState.appendChild(createEventState(), nullptr);
    }

    return projectState;
}

juce::String FileHelpers::getTestFileName()
{
    return "test-" + juce::Uuid().toString();
}
