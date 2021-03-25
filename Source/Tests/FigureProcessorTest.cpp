#include "FigureProcessor.h"

#include "Identifiers.h"
#include "StateHelpers.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>

SCENARIO("Figure processor")
{
    using namespace aleatoric;

    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid sourceId;
    state.appendChild(sh::createSourceState(sourceId), nullptr);
    juce::Uuid p1Id;
    juce::Uuid p2Id;
    juce::Uuid p3Id;
    juce::Uuid p4Id;
    auto particleOne = sh::createParticleState(p1Id, sourceId);
    auto particleTwo = sh::createParticleState(p2Id, sourceId);
    auto particleThree = sh::createParticleState(p3Id, sourceId);
    auto particleFour = sh::createParticleState(p4Id, sourceId);
    state.appendChild(particleOne, nullptr);
    state.appendChild(particleTwo, nullptr);
    state.appendChild(particleThree, nullptr);
    state.appendChild(particleFour, nullptr);

    ProjectState projectState(state);

    auto particleList = projectState.getParticleList();
    REQUIRE(particleList.getObjects().size() == 4);

    auto figureList = projectState.getFigureList();
    REQUIRE(figureList.getObjects().size() == 0);

    auto eventListAll = projectState.getEventList();
    REQUIRE(eventListAll.getObjects().size() == 0);

    int numOfParticles = 8;
    std::vector<int> durations {100, 200};

    // Basic predictable results using prescribed durations and cycling of
    // durations and particles
    DurationsProducer durationsProducer(
        DurationProtocol::createPrescribed(durations),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    CollectionsProducer<Particle> collectionsProducer(
        particleList.getObjects(),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    FigureProcessor processor;
    auto figure = processor.composeFigure(numOfParticles,
                                          durationsProducer,
                                          collectionsProducer,
                                          projectState);

    auto eventListForFigure = projectState.getEventList(figure);

    THEN("The figure is added to the project state")
    {
        auto figures = figureList.getObjects();
        REQUIRE(figures.size() == 1);
        REQUIRE(figures[0].getId() == figure.getId());
    }

    THEN("The events added to project state should match number of events "
         "requested")
    {
        REQUIRE(eventListAll.getObjects().size() == numOfParticles);
        REQUIRE(eventListForFigure.getObjects().size() == numOfParticles);
    }

    SECTION("Check event data")
    {
        auto events = eventListForFigure.getObjects();

        std::vector<double> actualOnsets;
        std::vector<juce::Uuid> actualParticleIds;

        for(auto &event : events) {
            actualOnsets.push_back(event.getOnset());
            actualParticleIds.push_back(event.getParticleId());
        }

        THEN("First event should be at onset zero")
        {
            REQUIRE(events[0].getOnset() == 0);
        }

        THEN("Durations between onsets should match durations provided in "
             "cyclical form")
        {
            std::vector<double>
                expectedOnsets {0, 100, 300, 400, 600, 700, 900, 1000};

            REQUIRE_THAT(actualOnsets, Catch::Equals(expectedOnsets));
        }

        THEN("Particle ids selected should match ids provided in cyclical form")
        {
            std::vector<juce::Uuid> expectedParticleIds {p1Id,
                                                         p2Id,
                                                         p3Id,
                                                         p4Id,
                                                         p1Id,
                                                         p2Id,
                                                         p3Id,
                                                         p4Id};

            REQUIRE_THAT(actualParticleIds, Catch::Equals(expectedParticleIds));
        }
    }
}
