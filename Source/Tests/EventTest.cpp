#include "Event.h"

#include "ErrorTypes.h"
#include "Identifiers.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("Event: receive existing invalid state")
{
    auto sourceState = StateHelpers::createSourceState();

    SECTION("Incorrect type")
    {
        juce::ValueTree state(IDs::SOURCE);
        REQUIRE_THROWS_AS(Event(state, Figure(), Particle(Source(sourceState))),
                          ValueTreeInvalidType);
    }

    SECTION("Missing properties")
    {
        juce::ValueTree state(IDs::EVENT);

        SECTION("No id")
        {
            state.setProperty(IDs::onset, 2, nullptr);
            state.setProperty(IDs::particle_id,
                              juce::Uuid().toString(),
                              nullptr);
            state.setProperty(IDs::figure_id, juce::Uuid().toString(), nullptr);
            REQUIRE_THROWS_AS(
                Event(state, Figure(), Particle(Source(sourceState))),
                ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No onset")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::particle_id,
                              juce::Uuid().toString(),
                              nullptr);
            state.setProperty(IDs::figure_id, juce::Uuid().toString(), nullptr);
            REQUIRE_THROWS_AS(
                Event(state, Figure(), Particle(Source(sourceState))),
                ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No particle id")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::onset, 2, nullptr);
            state.setProperty(IDs::figure_id, juce::Uuid().toString(), nullptr);
            REQUIRE_THROWS_AS(
                Event(state, Figure(), Particle(Source(sourceState))),
                ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No figure id")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::onset, 2, nullptr);
            state.setProperty(IDs::particle_id,
                              juce::Uuid().toString(),
                              nullptr);
            REQUIRE_THROWS_AS(
                Event(state, Figure(), Particle(Source(sourceState))),
                ValueTreeCompulsoryPropertyMissing);
        }
    }

    SECTION("Unexpected props present")
    {
        auto state = StateHelpers::createEventState();
        state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);
        REQUIRE_THROWS_AS(Event(state, Figure(), Particle(Source(sourceState))),
                          ValueTreeUnexpectedPropertyReceived);
    }

    SECTION("Invalid data")
    {
        Source source(sourceState);
        Particle validParticle(source);
        Figure validFigure;

        auto state = StateHelpers::createEventState(juce::Uuid(),
                                                    1,
                                                    validParticle.getId(),
                                                    validFigure.getId());

        SECTION("Figure id does not match state figure_id")
        {
            REQUIRE_THROWS_AS(Event(state, Figure(), validParticle),
                              ObjectDependencyInvalid);
        }

        SECTION("Particle id does not match state particle_id")
        {
            REQUIRE_THROWS_AS(Event(state, validFigure, Particle(source)),
                              ObjectDependencyInvalid);
        }

        SECTION("Onset is less than 0")
        {
            state.setProperty(IDs::onset, -0.99, nullptr);
            REQUIRE_THROWS_AS(Event(state, validFigure, validParticle),
                              EventOnsetInvalid);
        }
    }
}

SCENARIO("Event: receive existing state: get data")
{
    using sh = StateHelpers;

    juce::Uuid id;
    Source source(sh::createSourceState());
    Particle particle(source);
    Figure figure;

    auto state = sh::createEventState(id, 2, particle.getId(), figure.getId());

    Event event(state, figure, particle);

    REQUIRE(event.getId() == id);
    REQUIRE(event.getOnset() == 2);
    REQUIRE(event.getParticleId() == particle.getId());
    REQUIRE(event.getParticle().getState() == particle.getState());
    REQUIRE(event.getFigureId() == figure.getId());
    REQUIRE(event.getFigure().getState() == figure.getState());
    REQUIRE(event.getState() == state);
}

SCENARIO("Event: create state")
{
    Source source(StateHelpers::createSourceState());
    Particle particle(source);
    Figure figure;

    SECTION("Invalid data: onset less than 0")
    {
        REQUIRE_THROWS_AS(Event(figure, particle, -0.99), EventOnsetInvalid);
    }

    SECTION("Getting data")
    {
        double onset = 123.4;
        Event event(figure, particle, onset);

        REQUIRE_FALSE(event.getId().isNull());
        REQUIRE(event.getOnset() == onset);
        REQUIRE(event.getParticleId() == particle.getId());
        REQUIRE(event.getParticle().getState() == particle.getState());
        REQUIRE(event.getFigureId() == figure.getId());
        REQUIRE(event.getFigure().getState() == figure.getState());

        // Underlying state
        auto returnedState = event.getState();
        juce::Uuid id(returnedState[IDs::id]);
        double returnedOnset {returnedState[IDs::onset]};
        juce::Uuid particleId(returnedState[IDs::particle_id]);
        juce::Uuid figureId(returnedState[IDs::figure_id]);

        REQUIRE_FALSE(id.isNull());
        REQUIRE(returnedOnset == onset);
        REQUIRE(particleId == particle.getId());
        REQUIRE(figureId == figure.getId());
    }
}

SCENARIO("Event: set onset")
{
    Source source(StateHelpers::createSourceState());
    Particle particle(source);
    Figure figure;
    double onset = 123.4;
    Event event1(figure, particle, onset);

    SECTION("new onset is less than 0")
    {
        REQUIRE_THROWS_AS(event1.setOnset(-0.99), EventOnsetInvalid);
    }

    SECTION("new onset is valid")
    {
        bool evt1CbCalled = false;
        juce::Identifier evt1PropChanged;
        event1.onUpdated = [&evt1CbCalled, &evt1PropChanged](
                               juce::Identifier propertyChanged) {
            evt1CbCalled = true;
            evt1PropChanged = propertyChanged;
        };

        Event event2(event1.getState(), figure, particle);
        bool evt2CbCalled = false;
        juce::Identifier evt2PropChanged;
        event2.onUpdated = [&evt2CbCalled, &evt2PropChanged](
                               juce::Identifier propertyChanged) {
            evt2CbCalled = true;
            evt2PropChanged = propertyChanged;
        };

        event1.setOnset(567.8);
        REQUIRE(event1.getOnset() == 567.8);

        THEN("clients listening for propery changes are notified")
        {
            REQUIRE(evt1CbCalled);
            REQUIRE(evt1PropChanged == IDs::onset);
            REQUIRE(evt2CbCalled);
            REQUIRE(evt2PropChanged == IDs::onset);
        }
    }
}
