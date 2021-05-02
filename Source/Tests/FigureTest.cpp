#include "Figure.h"

#include "Identifiers.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("Figure: receive existing state")
{
    WHEN("Data is invalid")
    {
        SECTION("Incorrect type")
        {
            juce::ValueTree state(IDs::SOURCE);
            REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
        }

        SECTION("Missing properties")
        {
            juce::ValueTree state(IDs::FIGURE);

            SECTION("No id")
            {
                REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
            }
        }

        SECTION("Unexpected props present")
        {
            auto state = StateHelpers::createFigureState();
            state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);
            REQUIRE_THROWS_AS(Figure(state), std::invalid_argument);
        }
    }

    WHEN("Data is valid")
    {
        juce::Uuid id;
        auto state = StateHelpers::createFigureState(id);

        Figure figure(state);

        THEN("getting data")
        {
            REQUIRE(figure.getId() == id);
        }
    }
}

SCENARIO("Figure: create state")
{
    Figure figure;

    THEN("Getting data")
    {
        REQUIRE_FALSE(figure.getId().isNull());
        auto returnedState = figure.getState();
        REQUIRE(returnedState.hasType(IDs::FIGURE));
        REQUIRE(juce::Uuid(returnedState[IDs::id]) == figure.getId());
    }
}
