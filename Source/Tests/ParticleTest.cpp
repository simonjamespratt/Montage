#include "Particle.h"

#include "ErrorTypes.h"
#include "Identifiers.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("Particle: receive existing invalid state")
{
    SECTION("Incorrect type")
    {
        juce::ValueTree state(IDs::SOURCE);
        Source source(StateHelpers::createSourceState(juce::Uuid()));
        REQUIRE_THROWS_AS(Particle(state, source), ValueTreeInvalidType);
    }

    SECTION("Missing properties")
    {
        juce::ValueTree state(IDs::PARTICLE);
        juce::Uuid sourceId;
        Source source(StateHelpers::createSourceState(sourceId));
        double start = 0;
        double end = source.getFileLengthInSeconds();
        juce::String name("name");

        SECTION("No id")
        {
            state.setProperty(IDs::source_id, sourceId.toString(), nullptr);
            state.setProperty(IDs::start, start, nullptr);
            state.setProperty(IDs::end, end, nullptr);
            state.setProperty(IDs::name, name, nullptr);

            REQUIRE_THROWS_AS(Particle(state, source),
                              ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No source id")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::start, start, nullptr);
            state.setProperty(IDs::end, end, nullptr);
            state.setProperty(IDs::name, name, nullptr);

            REQUIRE_THROWS_AS(Particle(state, source),
                              ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No start")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::source_id, sourceId.toString(), nullptr);
            state.setProperty(IDs::end, end, nullptr);
            state.setProperty(IDs::name, name, nullptr);

            REQUIRE_THROWS_AS(Particle(state, source),
                              ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No end")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::source_id, sourceId.toString(), nullptr);
            state.setProperty(IDs::start, start, nullptr);
            state.setProperty(IDs::name, name, nullptr);

            REQUIRE_THROWS_AS(Particle(state, source),
                              ValueTreeCompulsoryPropertyMissing);
        }

        SECTION("No name")
        {
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
            state.setProperty(IDs::source_id, sourceId.toString(), nullptr);
            state.setProperty(IDs::start, start, nullptr);
            state.setProperty(IDs::end, end, nullptr);

            REQUIRE_THROWS_AS(Particle(state, source),
                              ValueTreeCompulsoryPropertyMissing);
        }
    }

    SECTION("Unexpected props present")
    {
        Source source(StateHelpers::createSourceState(juce::Uuid()));
        juce::ValueTree state(IDs::PARTICLE);
        state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
        state.setProperty(IDs::source_id, source.getId().toString(), nullptr);
        state.setProperty(IDs::start, 0, nullptr);
        state.setProperty(IDs::end, source.getFileLengthInSeconds(), nullptr);
        state.setProperty(IDs::name, juce::String("name"), nullptr);

        state.setProperty(IDs::SOURCE, "Unexpected prop", nullptr);

        REQUIRE_THROWS_AS(Particle(state, source),
                          ValueTreeUnexpectedPropertyReceived);
    }

    SECTION("Invalid data")
    {
        juce::Uuid sourceId;
        Source source(StateHelpers::createSourceState(sourceId));

        SECTION("Source id does not match state source_id")
        {
            auto particleState =
                StateHelpers::createParticleState(juce::Uuid(), juce::Uuid());

            REQUIRE_THROWS_AS(Particle(particleState, source),
                              ObjectDependencyInvalid);
        }

        SECTION("Start is less than 0")
        {
            auto particleState = StateHelpers::createParticleState(juce::Uuid(),
                                                                   sourceId,
                                                                   -0.99,
                                                                   1.0);
            REQUIRE_THROWS_AS(Particle(particleState, source),
                              ParticleStartInvalid);
        }

        SECTION("Start is not less than end")
        {
            REQUIRE_THROWS_AS(
                Particle(StateHelpers::createParticleState(juce::Uuid(),
                                                           sourceId,
                                                           0.2,
                                                           0.1),
                         source),
                ParticleStartInvalid);

            REQUIRE_THROWS_AS(
                Particle(StateHelpers::createParticleState(juce::Uuid(),
                                                           sourceId,
                                                           0.1,
                                                           0.1),
                         source),
                ParticleStartInvalid);
        }

        SECTION("End is greater than source file length")
        {
            auto fileLength = source.getFileLengthInSeconds();
            REQUIRE_THROWS_AS(
                Particle(StateHelpers::createParticleState(juce::Uuid(),
                                                           sourceId,
                                                           0.0,
                                                           fileLength + 0.1),
                         source),
                ParticleEndInvalid);
        }

        SECTION("Name is empty")
        {
            REQUIRE_THROWS_AS(Particle(StateHelpers::createParticleState(
                                           juce::Uuid(),
                                           sourceId,
                                           0.0,
                                           source.getFileLengthInSeconds(),
                                           juce::String("")),
                                       source),
                              ObjectNameInvalid);
        }
    }
}

SCENARIO("Particle: receive existing state: get data")
{
    juce::Uuid id;
    juce::Uuid sourceId;
    auto state = StateHelpers::createParticleState(id,
                                                   sourceId,
                                                   0.0,
                                                   1.0,
                                                   juce::String("test-name"));

    Source source(StateHelpers::createSourceState(sourceId));

    Particle particle(state, source);

    REQUIRE(particle.getId() == id);
    REQUIRE(particle.getSourceId() == sourceId);
    REQUIRE(particle.getStart() == 0.0);
    REQUIRE(particle.getEnd() == 1.0);
    REQUIRE(particle.getSource().getState() == source.getState());
    REQUIRE(particle.getName() == "test-name");

    auto returnedState = particle.getState();
    juce::Uuid returnedId(returnedState[IDs::id]);
    juce::Uuid returnedSourceId(returnedState[IDs::source_id]);
    double start {returnedState[IDs::start]};
    double end {returnedState[IDs::end]};
    juce::String name = returnedState[IDs::name];

    REQUIRE(returnedId == id);
    REQUIRE(returnedSourceId == sourceId);
    REQUIRE(start == 0.0);
    REQUIRE(end == 1.0);
    REQUIRE(name == "test-name");
}

SCENARIO("Particle: create state")
{
    juce::Uuid sourceId;
    Source source(StateHelpers::createSourceState(sourceId));

    Particle particle(source);

    THEN("start should be set to 0")
    {
        REQUIRE(particle.getStart() == 0);
    }

    THEN("end should be set to source file length")
    {
        REQUIRE(particle.getEnd() == source.getFileLengthInSeconds());
    }

    THEN("sourceId should be set to the incoming source id")
    {
        REQUIRE(particle.getSourceId() == sourceId);
    }

    THEN("particle id is set")
    {
        REQUIRE_FALSE(particle.getId().isNull());
    }

    THEN("name should be set to untitled")
    {
        REQUIRE(particle.getName() == "untitled");
    }

    THEN("Underlying state is as expected")
    {
        auto returnedState = particle.getState();
        juce::Uuid id(returnedState[IDs::id]);
        juce::Uuid sourceId(returnedState[IDs::source_id]);
        double start {returnedState[IDs::start]};
        double end {returnedState[IDs::end]};
        juce::String name = returnedState[IDs::name];

        REQUIRE_FALSE(id.isNull());
        REQUIRE(sourceId == source.getId());
        REQUIRE(start == 0.0);
        REQUIRE(end == source.getFileLengthInSeconds());
        REQUIRE(name == "untitled");
    }
}

SCENARIO("Particle: set start")
{
    juce::Uuid sourceId;
    Source source(StateHelpers::createSourceState(sourceId));

    auto state =
        StateHelpers::createParticleState(juce::Uuid(), sourceId, 0.0, 1.0);
    bool callbackHasBeenCalled = false;
    juce::Identifier propChanged;

    Particle particle(state, source);
    particle.onUpdated = [&callbackHasBeenCalled,
                          &propChanged](juce::Identifier propertyChanged) {
        callbackHasBeenCalled = true;
        propChanged = propertyChanged;
    };

    bool otherCallbackHasBeenCalled = false;
    juce::Identifier otherPropChanged;

    Particle otherParticle(state, source);
    otherParticle.onUpdated = [&otherCallbackHasBeenCalled, &otherPropChanged](
                                  juce::Identifier propertyChanged) {
        otherCallbackHasBeenCalled = true;
        otherPropChanged = propertyChanged;
    };

    SECTION("where new start is less than 0")
    {
        REQUIRE_THROWS_AS(particle.setStart(-0.99), ParticleStartInvalid);
    }

    SECTION("where new start is not less than end")
    {
        REQUIRE_THROWS_AS(particle.setStart(1.0), ParticleStartInvalid);
        REQUIRE_THROWS_AS(particle.setStart(1.01), ParticleStartInvalid);
    }

    SECTION("where new start is valid")
    {
        particle.setStart(0.5);
        REQUIRE(particle.getStart() == 0.5);
        REQUIRE(callbackHasBeenCalled);
        REQUIRE(propChanged == IDs::start);
        REQUIRE(otherCallbackHasBeenCalled);
        REQUIRE(otherPropChanged == IDs::start);
    }
}

SCENARIO("Particle: set end")
{
    juce::Uuid sourceId;
    Source source(StateHelpers::createSourceState(sourceId));
    double sourceFileLength = source.getFileLengthInSeconds();

    auto particleState = StateHelpers::createParticleState(juce::Uuid(),
                                                           sourceId,
                                                           1.0,
                                                           sourceFileLength);

    Particle particle(particleState, source);
    bool callbackHasBeenCalled;
    juce::Identifier propChanged;
    particle.onUpdated = [&callbackHasBeenCalled,
                          &propChanged](juce::Identifier propertyChanged) {
        callbackHasBeenCalled = true;
        propChanged = propertyChanged;
    };

    Particle otherParticle(particleState, source);
    bool otherCallbackHasBeenCalled;
    juce::Identifier otherPropChanged;
    otherParticle.onUpdated = [&otherCallbackHasBeenCalled, &otherPropChanged](
                                  juce::Identifier propertyChanged) {
        otherCallbackHasBeenCalled = true;
        otherPropChanged = propertyChanged;
    };

    SECTION("where new end is less than or equal to start")
    {
        REQUIRE_THROWS_AS(particle.setEnd(0.99), ParticleEndInvalid);
        REQUIRE_THROWS_AS(particle.setEnd(1.0), ParticleEndInvalid);
    }

    SECTION("where new end is greater than source file length")
    {
        REQUIRE_THROWS_AS(particle.setEnd(sourceFileLength + 0.01),
                          ParticleEndInvalid);
    }

    SECTION("where new end is valid")
    {
        auto newEnd = sourceFileLength - 0.01;
        particle.setEnd(newEnd);
        REQUIRE(particle.getEnd() == newEnd);
        REQUIRE(callbackHasBeenCalled);
        REQUIRE(propChanged == IDs::end);
        REQUIRE(otherCallbackHasBeenCalled);
        REQUIRE(otherPropChanged == IDs::end);
    }
}

SCENARIO("Particle: ensureNewStartIsWithinBounds")
{
    Source source(StateHelpers::createSourceState());
    double sourceFileLength = source.getFileLengthInSeconds();
    Particle particle(source);
    double existingEnd = 1.5;

    WHEN("proposed start is valid")
    {
        double proposedStart = 0.01;
        auto result =
            particle.ensureNewStartIsWithinBounds(proposedStart, existingEnd);

        THEN("returns copy of value unchanged")
        {
            REQUIRE(result == proposedStart);
        }
    }

    WHEN("proposed start is < 0")
    {
        double proposedStart = -0.99;
        auto result =
            particle.ensureNewStartIsWithinBounds(proposedStart, existingEnd);

        THEN("returns 0")
        {
            REQUIRE(result == 0);
        }
    }

    WHEN("proposed start is not <= 10ms less than end")
    {
        double proposedStart = existingEnd - 0.009; // 9 ms
        auto result =
            particle.ensureNewStartIsWithinBounds(proposedStart, existingEnd);

        THEN("retuns end - 10ms")
        {
            REQUIRE(result == (existingEnd - 0.01));
        }
    }
}

SCENARIO("Particle: ensureNewEndIsWithinBounds")
{
    Source source(StateHelpers::createSourceState());
    double sourceFileLength = source.getFileLengthInSeconds();
    Particle particle(source);
    double existingStart = 0.5;

    WHEN("proposed end is valid")
    {
        double proposedEnd = 1;
        auto result =
            particle.ensureNewEndIsWithinBounds(existingStart, proposedEnd);

        THEN("returns copy of value unchanged")
        {
            REQUIRE(result == proposedEnd);
        }
    }

    WHEN("proposed end is > source file length")
    {
        auto fileLength = particle.getSource().getFileLengthInSeconds();
        double proposedEnd = fileLength + 0.01;
        auto result =
            particle.ensureNewEndIsWithinBounds(existingStart, proposedEnd);

        THEN("returns file length")
        {
            REQUIRE(result == fileLength);
        }
    }

    WHEN("proposed end is not >= 10ms more than start")
    {
        double proposedEnd = existingStart + 0.009; // 9 ms
        auto result =
            particle.ensureNewEndIsWithinBounds(existingStart, proposedEnd);

        THEN("retuns start + 10ms")
        {
            REQUIRE(result == (existingStart + 0.01));
        }
    }
}

SCENARIO("Particle: setStartAndEnd")
{
    Source source(StateHelpers::createSourceState());
    double sourceFileLength = source.getFileLengthInSeconds();
    Particle particle(source);
    particle.setStart(0.5);
    particle.setEnd(1.5);

    double validStart = 0.75;
    double validEnd = 1.25;

    SECTION("Invalid data")
    {
        SECTION("Start is less than 0")
        {
            REQUIRE_THROWS_AS(particle.setStartAndEnd(-0.99, validEnd),
                              ParticleStartInvalid);
        }

        SECTION("Start is not less than end")
        {
            REQUIRE_THROWS_AS(particle.setStartAndEnd(1.26, validEnd),
                              ParticleStartInvalid);
            REQUIRE_THROWS_AS(particle.setStartAndEnd(1.25, validEnd),
                              ParticleStartInvalid);
        }

        SECTION("End is greater than source file length")
        {
            REQUIRE_THROWS_AS(
                particle.setStartAndEnd(validStart, (sourceFileLength + 0.001)),
                ParticleEndInvalid);
        }
    }

    SECTION("Valid data")
    {
        particle.setStartAndEnd(validStart, validEnd);

        THEN("start and end are correctly set")
        {
            REQUIRE(particle.getStart() == validStart);
            REQUIRE(particle.getEnd() == validEnd);
        }
    }

    SECTION("Valid data that could cause breach of invariants")
    {
        WHEN("New start and end are greater than current end")
        {
            // setting the vt state's start property before the end property
            // would cause an exception to be thrown, should a
            // state-change-listener try to create a Particle from that interim
            // vt state in its callback. This is because the new start is
            // greater than the current end. The constructor detects this during
            // its validation

            double newHighStart = particle.getEnd() + 0.1;
            double newHighEnd = particle.getEnd() + 0.2;

            THEN("Exceptions should not be thrown when creating Particles in "
                 "listener callbacks")
            {
                particle.onUpdated = [&particle, &source](juce::Identifier) {
                    REQUIRE_NOTHROW(Particle(particle.getState(), source));
                };

                particle.setStartAndEnd(newHighStart, newHighEnd);
            }
        }

        WHEN("New end is less than current start")
        {
            // same as above but this time setting vt state's end prop before
            // start prop would cause the interim breach of invariant rules

            double newLowStart = particle.getEnd() - 0.2;
            double newLowEnd = particle.getEnd() - 0.1;

            THEN("Exceptions should not be thrown when creating Particles in "
                 "listener callbacks")
            {
                particle.onUpdated = [&particle, &source](juce::Identifier) {
                    REQUIRE_NOTHROW(Particle(particle.getState(), source));
                };

                particle.setStartAndEnd(newLowStart, newLowEnd);
            }
        }
    }
}

SCENARIO("Particle: set name")
{
    juce::Uuid sourceId;
    Source source(StateHelpers::createSourceState(sourceId));
    double sourceFileLength = source.getFileLengthInSeconds();

    auto particleState =
        StateHelpers::createParticleState(juce::Uuid(),
                                          sourceId,
                                          1.0,
                                          sourceFileLength,
                                          juce::String("initial name"));

    Particle particle(particleState, source);
    REQUIRE(particle.getName() == "initial name");

    bool callbackHasBeenCalled;
    juce::Identifier propChanged;
    particle.onUpdated = [&callbackHasBeenCalled,
                          &propChanged](juce::Identifier propertyChanged) {
        callbackHasBeenCalled = true;
        propChanged = propertyChanged;
    };

    Particle otherParticle(particleState, source);
    bool otherCallbackHasBeenCalled;
    juce::Identifier otherPropChanged;
    otherParticle.onUpdated = [&otherCallbackHasBeenCalled, &otherPropChanged](
                                  juce::Identifier propertyChanged) {
        otherCallbackHasBeenCalled = true;
        otherPropChanged = propertyChanged;
    };

    SECTION("where new name is an empty string")
    {
        REQUIRE_THROWS_AS(particle.setName(""), ObjectNameInvalid);
    }

    SECTION("where new name is valid")
    {
        juce::String newName = "new name";
        particle.setName(newName);
        REQUIRE(particle.getName() == newName);
        REQUIRE(callbackHasBeenCalled);
        REQUIRE(propChanged == IDs::name);
        REQUIRE(otherCallbackHasBeenCalled);
        REQUIRE(otherPropChanged == IDs::name);
    }
}
