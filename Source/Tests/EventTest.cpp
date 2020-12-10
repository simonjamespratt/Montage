#include "Event.h"

#include "Identifiers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

TEST_CASE("Event: invalid data type throws error")
{
    juce::ValueTree state(IDs::SOURCE);

    REQUIRE_THROWS_AS(Event(state), std::invalid_argument);
}

TEST_CASE("Event: invalid properties throw errors")
{
    juce::ValueTree state(IDs::EVENT);

    SECTION("No id")
    {
        state.setProperty(IDs::onset, 2, nullptr);
        state.setProperty(IDs::particle_id, 3, nullptr);
        REQUIRE_THROWS_AS(Event(state), std::invalid_argument);
    }

    SECTION("No onset")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::particle_id, 3, nullptr);
        REQUIRE_THROWS_AS(Event(state), std::invalid_argument);
    }

    SECTION("No particle id")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::onset, 2, nullptr);
        REQUIRE_THROWS_AS(Event(state), std::invalid_argument);
    }

    SECTION("Unexpected props present")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::onset, 2, nullptr);
        state.setProperty(IDs::particle_id, 3, nullptr);
        state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);

        REQUIRE_THROWS_AS(Event(state), std::invalid_argument);
    }
}

TEST_CASE("Event: get data")
{
    juce::ValueTree state(IDs::EVENT);
    state.setProperty(IDs::id, 1, nullptr);
    state.setProperty(IDs::onset, 2, nullptr);
    state.setProperty(IDs::particle_id, 3, nullptr);

    Event event(state);

    REQUIRE(event.getId() == 1);
    REQUIRE(event.getOnset() == 2);
    REQUIRE(event.getParticleId() == 3);
}
