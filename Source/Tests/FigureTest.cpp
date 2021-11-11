#include "Figure.h"

#include "ErrorTypes.h"
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
            REQUIRE_THROWS_AS(Figure(state), ValueTreeInvalidType);
        }

        SECTION("Missing properties")
        {
            juce::ValueTree state(IDs::FIGURE);
            juce::String name("figure name");

            SECTION("No id")
            {
                state.setProperty(IDs::name, name, nullptr);
                REQUIRE_THROWS_AS(Figure(state),
                                  ValueTreeCompulsoryPropertyMissing);
            }

            SECTION("No name")
            {
                state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
                REQUIRE_THROWS_AS(Figure(state),
                                  ValueTreeCompulsoryPropertyMissing);
            }
        }

        SECTION("Unexpected props present")
        {
            auto state = StateHelpers::createFigureState();
            state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);
            REQUIRE_THROWS_AS(Figure(state),
                              ValueTreeUnexpectedPropertyReceived);
        }

        SECTION("Invalid data")
        {
            SECTION("Name is empty")
            {
                REQUIRE_THROWS_AS(
                    Figure(StateHelpers::createFigureState(juce::Uuid(), "")),
                    ObjectNameInvalid);
            }
        }
    }

    WHEN("Data is valid")
    {
        juce::Uuid id;
        juce::String name("test-name");
        auto state = StateHelpers::createFigureState(id, name);

        Figure figure(state);

        THEN("getting data")
        {
            REQUIRE(figure.getId() == id);
            REQUIRE(figure.getName() == name);
        }
    }
}

SCENARIO("Figure: create state")
{
    Figure figure;

    THEN("name is set to untitled")
    {
        REQUIRE(figure.getName() == "untitled");
    }

    THEN("Underlying state is as expected")
    {
        REQUIRE_FALSE(figure.getId().isNull());
        auto returnedState = figure.getState();
        REQUIRE(returnedState.hasType(IDs::FIGURE));
        REQUIRE(juce::Uuid(returnedState[IDs::id]) == figure.getId());
        REQUIRE(returnedState[IDs::name] == "untitled");
    }
}

SCENARIO("Figure: set name")
{
    Figure figure;

    REQUIRE(figure.getName() == "untitled");

    bool callbackHasBeenCalled;
    juce::Identifier propChanged;
    figure.onUpdated = [&callbackHasBeenCalled,
                        &propChanged](juce::Identifier propertyChanged) {
        callbackHasBeenCalled = true;
        propChanged = propertyChanged;
    };

    Figure otherFigure(figure.getState());
    bool otherCallbackHasBeenCalled;
    juce::Identifier otherPropChanged;
    otherFigure.onUpdated = [&otherCallbackHasBeenCalled, &otherPropChanged](
                                juce::Identifier propertyChanged) {
        otherCallbackHasBeenCalled = true;
        otherPropChanged = propertyChanged;
    };

    SECTION("when new name is an empty string")
    {
        REQUIRE_THROWS_AS(figure.setName(""), ObjectNameInvalid);
    }

    SECTION("when new name is valid")
    {
        juce::String newName("new name");
        figure.setName(newName);
        REQUIRE(figure.getName() == newName);
        REQUIRE(callbackHasBeenCalled);
        REQUIRE(propChanged == IDs::name);
        REQUIRE(otherCallbackHasBeenCalled);
        REQUIRE(otherPropChanged == IDs::name);
    }
}
