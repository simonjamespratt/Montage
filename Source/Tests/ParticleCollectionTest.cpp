#include "ParticleCollection.h"

#include "Identifiers.h"
#include "Particle.h"
#include "StateHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

TEST_CASE("Particles Collection: invalid data type throws error")
{
    juce::ValueTree state(IDs::SOURCE);

    REQUIRE_THROWS_AS(ParticleCollection(state), std::invalid_argument);
}

TEST_CASE("Particles Collection: children with invalid type throws error")
{
    juce::ValueTree state(IDs::PARTICLES);

    juce::ValueTree particle(IDs::PARTICLE);
    juce::ValueTree notAParticle(IDs::SOURCE);

    state.appendChild(particle, nullptr);
    state.appendChild(notAParticle, nullptr);

    REQUIRE_THROWS_AS(ParticleCollection(state), std::invalid_argument);
}

TEST_CASE("Particles Collection: get particles")
{
    juce::ValueTree state(IDs::PARTICLES);

    auto particleOne = StateHelpers::getParticleState(1);
    auto particleTwo = StateHelpers::getParticleState(2);

    state.appendChild(particleOne, nullptr);
    state.appendChild(particleTwo, nullptr);

    ParticleCollection particlesCollection(state);

    auto particles = particlesCollection.getParticles();

    REQUIRE(particles.size() == 2);
    REQUIRE(particles[0].getId() == 1);
    REQUIRE(particles[1].getId() == 2);
}
