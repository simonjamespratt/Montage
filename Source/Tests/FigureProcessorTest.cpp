#include "FigureProcessor.h"

#include "FigureCollection.h"
#include "Identifiers.h"
#include "ParticleCollection.h"
#include "StateHelpers.h"

#include <CollectionsProducer.hpp>
#include <DurationsProducer.hpp>
#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>

SCENARIO("Figure processor")
{
    using namespace aleatoric;

    juce::ValueTree particleCollectionState(IDs::PARTICLES);
    auto particleOne = StateHelpers::getParticleState(1);
    auto particleTwo = StateHelpers::getParticleState(2);
    auto particleThree = StateHelpers::getParticleState(3);
    auto particleFour = StateHelpers::getParticleState(4);
    particleCollectionState.appendChild(particleOne, nullptr);
    particleCollectionState.appendChild(particleTwo, nullptr);
    particleCollectionState.appendChild(particleThree, nullptr);
    particleCollectionState.appendChild(particleFour, nullptr);

    ParticleCollection particleCollection(particleCollectionState);

    juce::ValueTree figureCollectionState(IDs::FIGURES);
    FigureCollection figureCollection(figureCollectionState);

    // assert that the figureCollection has no figures yet
    REQUIRE(figureCollection.getFigures().size() == 0);

    int numOfParticles = 8;
    std::vector<int> durations {100, 200};

    // Basic predictable results using prescribed durations and cycling of
    // durations and particles
    DurationsProducer durationsProducer(
        DurationProtocol::createPrescribed(durations),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    CollectionsProducer<Particle> collectionsProducer(
        particleCollection.getParticles(),
        NumberProtocol::create(NumberProtocol::Type::cycle));

    FigureProcessor processor;
    auto figure = processor.composeFigure(numOfParticles,
                                          durationsProducer,
                                          collectionsProducer,
                                          figureCollection);

    auto events = figure.getEvents();

    THEN("Events returned should match number of particles requested")
    {
        REQUIRE(events.size() == numOfParticles);
    }

    SECTION("Check event data")
    {
        std::vector<int> actualIds;
        std::vector<double> actualOnsets;
        std::vector<int> actualParticleIds;

        for(auto &&event : events) {
            actualIds.push_back(event.getId());
            actualOnsets.push_back(event.getOnset());
            actualParticleIds.push_back(event.getParticleId());
        }

        THEN("Event ids should be incremental")
        {
            std::vector<int> expectedIds {1, 2, 3, 4, 5, 6, 7, 8};
            REQUIRE_THAT(actualIds, Catch::Equals(expectedIds));
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
            std::vector<int> expectedParticleIds {1, 2, 3, 4, 1, 2, 3, 4};
            REQUIRE_THAT(actualParticleIds, Catch::Equals(expectedParticleIds));
        }
    }

    SECTION("Figure collection updating")
    {
        THEN("The figure is added to the collection")
        {
            REQUIRE(figureCollection.getFigures().size() == 1);
        }
    }
}
