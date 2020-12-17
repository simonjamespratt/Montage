#include "FigureCollection.h"

#include "Identifiers.h"
#include "StateHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

TEST_CASE("Figure collection: invalid data type throws error")
{
    juce::ValueTree state(IDs::SOURCE);

    REQUIRE_THROWS_AS(FigureCollection(state), std::invalid_argument);
}

TEST_CASE("Figure collection: children with invalid type throws error")
{
    juce::ValueTree state(IDs::FIGURES);

    juce::ValueTree figure(IDs::FIGURE);
    juce::ValueTree notFigure(IDs::SOURCE);

    state.appendChild(figure, nullptr);
    state.appendChild(notFigure, nullptr);

    REQUIRE_THROWS_AS(FigureCollection(state), std::invalid_argument);
}

TEST_CASE("Figure collection: get figures")
{
    juce::ValueTree state(IDs::FIGURES);
    auto figureOne = StateHelpers::getFigureState(1);
    auto figureTwo = StateHelpers::getFigureState(2);
    state.appendChild(figureOne, nullptr);
    state.appendChild(figureTwo, nullptr);

    FigureCollection figureCollection(state);
    auto figures = figureCollection.getFigures();

    REQUIRE(figures.size() == 2);
    REQUIRE(figures[0].getId() == 1);
    REQUIRE(figures[1].getId() == 2);
}

TEST_CASE("Figure Collection: create figure")
{
    juce::ValueTree state(IDs::FIGURES);

    SECTION("Adding figure and returning it")
    {
        auto figureOne = StateHelpers::getFigureState(1);
        state.appendChild(figureOne, nullptr);

        FigureCollection figureCollection(state);

        auto initialFigures = figureCollection.getFigures();
        REQUIRE(initialFigures.size() == 1);

        auto figure = figureCollection.createFigure();
        auto updatedFigures = figureCollection.getFigures();
        REQUIRE(updatedFigures.size() == 2);
        REQUIRE(figure.getId() == 2);
    }

    SECTION("Set id for new event to highest existing id plus one")
    {
        auto figureStateOne = StateHelpers::getFigureState(4);
        auto figureStateTwo = StateHelpers::getFigureState(1);
        auto figureStateThree = StateHelpers::getFigureState(2);
        state.appendChild(figureStateOne, nullptr);
        state.appendChild(figureStateTwo, nullptr);
        state.appendChild(figureStateThree, nullptr);

        FigureCollection figureCollection(state);

        auto newFigure = figureCollection.createFigure();

        REQUIRE(newFigure.getId() == 5);
    }
}
