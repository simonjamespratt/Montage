#include "Particle.h"

#include "Identifiers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

TEST_CASE("Particle: invalid data type throws error")
{
    juce::ValueTree state(IDs::SOURCE);

    REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
}

TEST_CASE("Particle: invalid properties throws error")
{
    juce::ValueTree state(IDs::PARTICLE);

    SECTION("No id")
    {
        state.setProperty(IDs::source_id, 2, nullptr);
        state.setProperty(IDs::start, 100, nullptr);
        state.setProperty(IDs::end, 200, nullptr);

        REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
    }

    SECTION("No source id")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::start, 100, nullptr);
        state.setProperty(IDs::end, 200, nullptr);

        REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
    }

    SECTION("No start")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::source_id, 2, nullptr);
        state.setProperty(IDs::end, 200, nullptr);

        REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
    }

    SECTION("No end")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::source_id, 2, nullptr);
        state.setProperty(IDs::start, 100, nullptr);

        REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
    }

    SECTION("Unexpected props present")
    {
        juce::ValueTree state(IDs::PARTICLE);
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::source_id, 2, nullptr);
        state.setProperty(IDs::start, 100, nullptr);
        state.setProperty(IDs::end, 200, nullptr);

        state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);

        REQUIRE_THROWS_AS(Particle(state), std::invalid_argument);
    }
}

TEST_CASE("Particle: get data")
{
    juce::ValueTree state(IDs::PARTICLE);
    state.setProperty(IDs::id, 1, nullptr);
    state.setProperty(IDs::source_id, 2, nullptr);
    state.setProperty(IDs::start, 100, nullptr);
    state.setProperty(IDs::end, 200, nullptr);

    Particle particle(state);

    REQUIRE(particle.getId() == 1);
    REQUIRE(particle.getSourceId() == 2);
    REQUIRE(particle.getStart() == 100);
    REQUIRE(particle.getEnd() == 200);
}
