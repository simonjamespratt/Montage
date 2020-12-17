#include "Figure.h"

#include "Identifiers.h"
#include "StateHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

TEST_CASE("Figure: invalid data type throws error")
{
    juce::ValueTree state(IDs::SOURCE);

    REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
}

TEST_CASE("Figure: invalid properties throw errors")
{
    juce::ValueTree state(IDs::FIGURE);

    SECTION("No id")
    {
        REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
    }

    SECTION("Unexpected props present")
    {
        state.setProperty(IDs::id, 1, nullptr);
        state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);
        REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
    }
}

TEST_CASE("Figure: get data: own properties")
{
    auto state = StateHelpers::getFigureState(1);

    Figure figure(state);
    REQUIRE(figure.getId() == 1);
}

TEST_CASE("Figure: children with invalid type throw errors")
{
    auto state = StateHelpers::getFigureState(1);

    juce::ValueTree event(IDs::EVENT);
    juce::ValueTree notAnEvent(IDs::SOURCE);
    state.appendChild(event, nullptr);
    state.appendChild(notAnEvent, nullptr);

    REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
}

TEST_CASE("Figure: get events")
{
    auto state = StateHelpers::getFigureState(1);

    auto eventStateOne = StateHelpers::getEventState(1);
    auto eventStateTwo = StateHelpers::getEventState(2);
    state.appendChild(eventStateOne, nullptr);
    state.appendChild(eventStateTwo, nullptr);

    Figure figure(state);

    auto events = figure.getEvents();

    REQUIRE(events.size() == 2);
    REQUIRE(events[0].getId() == 1);
    REQUIRE(events[1].getId() == 2);
}

TEST_CASE("Figure: create event")
{
    auto state = StateHelpers::getFigureState(1);

    SECTION("Adding event and returning it")
    {
        auto eventStateOne = StateHelpers::getEventState(1);
        state.appendChild(eventStateOne, nullptr);

        Figure figure(state);
        auto events = figure.getEvents();

        REQUIRE(events.size() == 1);

        auto newEvent = figure.createEvent(123, 4567);
        auto updatedEvents = figure.getEvents();

        REQUIRE(updatedEvents.size() == 2);

        REQUIRE(newEvent.getId() == 2);
        REQUIRE(newEvent.getOnset() == 123);
        REQUIRE(newEvent.getParticleId() == 4567);
    }

    SECTION("Set id for new event to highest existing id plus one")
    {
        auto eventStateOne = StateHelpers::getEventState(4);
        auto eventStateTwo = StateHelpers::getEventState(1);
        auto eventStateThree = StateHelpers::getEventState(2);
        state.appendChild(eventStateOne, nullptr);
        state.appendChild(eventStateTwo, nullptr);
        state.appendChild(eventStateThree, nullptr);

        Figure figure(state);

        auto newEvent = figure.createEvent(100, 1000);

        REQUIRE(newEvent.getId() == 5);
    }
}
