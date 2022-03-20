#include "Figure.h"

#include "ErrorTypes.h"
#include "Identifiers.h"
#include "TestHelpers.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
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
            bool isGenerated = false;

            SECTION("No id")
            {
                state.setProperty(IDs::name, name, nullptr);
                state.setProperty(IDs::is_generated, isGenerated, nullptr);
                REQUIRE_THROWS_AS(Figure(state),
                                  ValueTreeCompulsoryPropertyMissing);
            }

            SECTION("No name")
            {
                state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
                state.setProperty(IDs::is_generated, isGenerated, nullptr);
                REQUIRE_THROWS_AS(Figure(state),
                                  ValueTreeCompulsoryPropertyMissing);
            }

            SECTION("No isGenerated")
            {
                state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
                state.setProperty(IDs::name, name, nullptr);
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

    THEN("isGenerated is set to false")
    {
        REQUIRE_FALSE(figure.getIsGenerated());
    }

    THEN("Underlying state is as expected")
    {
        REQUIRE_FALSE(figure.getId().isNull());
        auto returnedState = figure.getState();
        REQUIRE(returnedState.hasType(IDs::FIGURE));
        REQUIRE(juce::Uuid(returnedState[IDs::id]) == figure.getId());
        REQUIRE(returnedState[IDs::name] == "untitled");
        REQUIRE(bool(returnedState[IDs::is_generated]) == false);
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

SCENARIO("Figure:: set isGenerated")
{
    Figure figure;

    REQUIRE_FALSE(figure.getIsGenerated());

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

    SECTION("is sets the value correctly")
    {
        figure.setIsGenerated(true);
        REQUIRE(figure.getIsGenerated() == true);
        REQUIRE(callbackHasBeenCalled);
        REQUIRE(propChanged == IDs::is_generated);
        REQUIRE(otherCallbackHasBeenCalled);
        REQUIRE(otherPropChanged == IDs::is_generated);
    }
}

int stateCount(juce::Identifier id, juce::ValueTree v)
{
    int noOfStatesWithId = 0;
    for(int i = 0; i < v.getNumChildren(); i++) {
        auto child = v.getChild(i);
        if(child.hasType(id)) {
            noOfStatesWithId++;
        }
    }

    return noOfStatesWithId;
};

SCENARIO("Figure: set and get creation settings")
{
    Figure f;

    SECTION("get before set returns nullptr")
    {
        REQUIRE(f.getCreationSettings() == nullptr);
    }

    SECTION("set then get - returns correct values")
    {
        DurationProtocolSettings newDurations;
        newDurations.activeType = DurationProtocolType::geometric;
        newDurations.geometric.rangeStart = 100;
        newDurations.geometric.rangeEnd = 300;
        newDurations.geometric.collectionSize = 400;

        aleatoric::DurationsProducer durationsProducer(
            aleatoric::DurationProtocol::createGeometric(
                aleatoric::Range(1, 10),
                10),
            aleatoric::NumberProtocol::create(
                aleatoric::NumberProtocol::Type::cycle));
        durationsProducer.setParams(aleatoric::CycleParams(true, true));

        auto durationsSelectionParams = durationsProducer.getParams();
        NumberProtocolSettings newDurationsSelections(
            durationsSelectionParams,
            CreationContext::duration_selection);

        aleatoric::CollectionsProducer<int> particlesProducer(
            std::vector<int> {1, 2, 3},
            aleatoric::NumberProtocol::create(
                aleatoric::NumberProtocol::Type::cycle));
        particlesProducer.setParams(aleatoric::CycleParams(true, true));

        auto particleSelectionParams = particlesProducer.getParams();
        NumberProtocolSettings newParticlesSelections(
            particleSelectionParams,
            CreationContext::particle_selection);

        FigureCreationSettings newSettings(newDurations,
                                           newDurationsSelections,
                                           newParticlesSelections);

        f.setCreationSettings(newSettings);
        auto returnedSettings = f.getCreationSettings();

        REQUIRE(returnedSettings->durations.activeType ==
                DurationProtocolType::geometric);
        REQUIRE(returnedSettings->durations.geometric.rangeStart == 100);
        REQUIRE(returnedSettings->durations.geometric.rangeEnd == 300);
        REQUIRE(returnedSettings->durations.geometric.collectionSize == 400);

        REQUIRE(returnedSettings->durationSelection.context ==
                CreationContext::duration_selection);
        REQUIRE(returnedSettings->durationSelection.activeType ==
                NumberProtocolType::cycle);
        REQUIRE(returnedSettings->durationSelection.cycle.bidirectional ==
                true);
        REQUIRE(returnedSettings->durationSelection.cycle.reverseDirection ==
                true);

        REQUIRE(returnedSettings->particleSelection.context ==
                CreationContext::particle_selection);
        REQUIRE(returnedSettings->particleSelection.activeType ==
                NumberProtocolType::cycle);
        REQUIRE(returnedSettings->particleSelection.cycle.bidirectional ==
                true);
        REQUIRE(returnedSettings->particleSelection.cycle.reverseDirection ==
                true);

        SECTION("set again replaces settings state rather than adds another")
        {
            auto stateBefore = f.getState();

            REQUIRE(stateCount(IDs::DURATION_SETTINGS, stateBefore) == 1);
            REQUIRE(stateCount(IDs::NUMBER_SETTINGS, stateBefore) == 2);

            FigureCreationSettings moreSettings(
                (DurationProtocolSettings()),
                (NumberProtocolSettings(juce::ValueTree(IDs::NUMBER_SETTINGS))),
                (NumberProtocolSettings(
                    juce::ValueTree(IDs::NUMBER_SETTINGS))));

            f.setCreationSettings(moreSettings);

            auto stateAfter = f.getState();

            REQUIRE(stateCount(IDs::DURATION_SETTINGS, stateAfter) == 1);
            REQUIRE(stateCount(IDs::NUMBER_SETTINGS, stateAfter) == 2);
        }
    }
}
