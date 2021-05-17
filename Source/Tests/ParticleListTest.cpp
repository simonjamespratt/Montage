#include "ErrorTypes.h"
#include "ProjectState.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("ParticleList: getObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);

    WHEN("state has mixed entities but no particles")
    {
        ProjectState projectState(state);
        auto particleList = projectState.getParticleList();

        THEN("returns no objects")
        {
            REQUIRE(particleList.getObjects().size() == 0);
        }
    }

    WHEN("state has mixed entities including particles")
    {
        juce::Uuid p1Id;
        juce::Uuid p2Id;
        juce::Uuid s1Id;
        juce::Uuid s2Id;
        state.appendChild(sh::createSourceState(s1Id), nullptr);
        state.appendChild(sh::createSourceState(s2Id), nullptr);
        state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);
        state.appendChild(sh::createParticleState(p2Id, s2Id), nullptr);

        ProjectState projectState(state);
        auto particleList = projectState.getParticleList();

        THEN("returns the particles")
        {
            auto objects = particleList.getObjects();
            REQUIRE(objects.size() == 2);
            REQUIRE(objects[0].getId() == p1Id);
            REQUIRE(objects[1].getId() == p2Id);
        }
    }
}

SCENARIO("ParticleList: getIndex")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid s1Id;
    juce::Uuid p1Id;
    juce::Uuid p2Id;
    juce::Uuid p3Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);
    state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);
    state.appendChild(sh::createParticleState(p2Id, s1Id), nullptr);
    state.appendChild(sh::createParticleState(p3Id, s1Id), nullptr);

    ProjectState projectState(state);
    auto list = projectState.getParticleList();

    REQUIRE(list.getObjects().size() == 3);

    WHEN("Particle exists in the list")
    {
        auto particleToFind = list.getObjects()[1];

        THEN("returns the index")
        {
            REQUIRE(list.getIndex(particleToFind) == 1);
        }
    }

    WHEN("Particle cannot be found in list")
    {
        Source source(sourceState);
        Particle orphanParticle(source);

        THEN("returns -1")
        {
            REQUIRE(list.getIndex(orphanParticle) == -1);
        }
    }
}

SCENARIO("ParticleList: addObject")
{
    using sh = StateHelpers;

    juce::Uuid validSourceId;
    juce::Uuid eventId;

    juce::ValueTree state(IDs::PROJECT_STATE);
    auto validSourceState = sh::createSourceState(validSourceId);
    auto existingParticleState =
        sh::createParticleState(juce::Uuid(), validSourceId);

    state.appendChild(validSourceState, nullptr);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createEventState(eventId), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(existingParticleState, nullptr);
    state.appendChild(sh::createParticleState(juce::Uuid(), validSourceId),
                      nullptr);

    ProjectState projectState(state);

    // NB: get instances only after state set up above - order matters
    auto particleList = projectState.getParticleList();
    bool particleListCallbackHasBeenCalled = false;
    juce::Uuid particleListAddedParticleId;
    particleList.onObjectAdded =
        [&particleListCallbackHasBeenCalled,
         &particleListAddedParticleId](Particle newParticle) {
            particleListCallbackHasBeenCalled = true;
            particleListAddedParticleId = newParticle.getId();
        };

    auto otherParticleList = projectState.getParticleList();
    bool otherParticleListCallbackHasBeenCalled = false;
    juce::Uuid otherParticleListAddedParticleId;
    otherParticleList.onObjectAdded =
        [&otherParticleListCallbackHasBeenCalled,
         &otherParticleListAddedParticleId](Particle newParticle) {
            otherParticleListCallbackHasBeenCalled = true;
            otherParticleListAddedParticleId = newParticle.getId();
        };

    REQUIRE(particleList.getObjects().size() == 2);
    REQUIRE(otherParticleList.getObjects().size() == 2);

    WHEN("Validating")
    {
        SECTION("Particle's source is not in project state")
        {
            auto invalidSourceState =
                StateHelpers::createSourceState(juce::Uuid());
            Source source(invalidSourceState);
            Particle particle(source);

            REQUIRE_THROWS_AS(particleList.addObject(particle),
                              ObjectDependencyNotFound);
        }
    }

    WHEN("Params are valid and particle does not exist in project state")
    {
        Source source(validSourceState);
        Particle particle(source);
        auto returnedParticle = particleList.addObject(particle);

        THEN("particleList objects increases by one")
        {
            REQUIRE(particleList.getObjects().size() == 3);
        }

        THEN("other particle lists also reflect this")
        {
            REQUIRE(otherParticleList.getObjects().size() == 3);
        }

        THEN("Returned Particle's state reflects having been added")
        {
            auto returnedState = returnedParticle.getState();
            REQUIRE(returnedState.getParent().hasType(IDs::PROJECT_STATE));
            REQUIRE(returnedState == particle.getState());
        }

        THEN("clients listening for added particles are notified")
        {
            REQUIRE(particleListCallbackHasBeenCalled);
            REQUIRE(particleListAddedParticleId == particle.getId());
            REQUIRE(otherParticleListCallbackHasBeenCalled);
            REQUIRE(otherParticleListAddedParticleId == particle.getId());
        }

        WHEN("other entities are added to the state")
        {
            particleListCallbackHasBeenCalled = false;
            otherParticleListCallbackHasBeenCalled = false;
            state.appendChild(StateHelpers::createEventState(juce::Uuid()),
                              nullptr);

            THEN("clients listening for added particles are not notified")
            {
                REQUIRE_FALSE(particleListCallbackHasBeenCalled);
                REQUIRE_FALSE(otherParticleListCallbackHasBeenCalled);
            }
        }
    }

    WHEN("Params are valid and particle already exists in project state")
    {
        Source source(validSourceState);
        Particle particle(existingParticleState, source);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(particleList.addObject(particle),
                              ObjectAlreadyExists);
        }
    }
}

SCENARIO("ParticleList: removeObject")
{
    juce::ValueTree state(IDs::PROJECT_STATE);

    ProjectState projectState(state);
    juce::Uuid sourceId;
    auto sourceState = StateHelpers::createSourceState(sourceId);
    state.appendChild(sourceState, nullptr);

    auto eventState = StateHelpers::createEventState(juce::Uuid());
    state.appendChild(eventState, nullptr);

    auto listOne = projectState.getParticleList();
    bool listOneCallbackHasBeenCalled = false;
    juce::Uuid listOneCbId;
    listOne.onObjectRemoved = [&listOneCallbackHasBeenCalled,
                               &listOneCbId](Particle particleRemoved) {
        listOneCallbackHasBeenCalled = true;
        listOneCbId = particleRemoved.getId();
    };

    auto listTwo = projectState.getParticleList();
    bool listTwoCallbackHasBeenCalled = false;
    juce::Uuid listTwoCbId;
    listTwo.onObjectRemoved = [&listTwoCallbackHasBeenCalled,
                               &listTwoCbId](Particle particleRemoved) {
        listTwoCallbackHasBeenCalled = true;
        listTwoCbId = particleRemoved.getId();
    };

    REQUIRE(listOne.getObjects().size() == 0);
    REQUIRE(listTwo.getObjects().size() == 0);

    Source source(sourceState);
    Particle particle(source);

    listOne.addObject(particle);

    REQUIRE(listOne.getObjects().size() == 1);
    REQUIRE(listTwo.getObjects().size() == 1);

    REQUIRE_FALSE(listOneCallbackHasBeenCalled);
    REQUIRE_FALSE(listTwoCallbackHasBeenCalled);

    WHEN("Particle exists in project state")
    {
        auto returnedParticle = listOne.removeObject(particle);

        THEN("particle is removed from list of objects")
        {
            REQUIRE(listOne.getObjects().size() == 0);
            REQUIRE(listTwo.getObjects().size() == 0);
        }

        THEN("returned Particle's state reflects having been removed")
        {
            auto returnedState = returnedParticle.getState();
            REQUIRE_FALSE(returnedState.getParent().isValid());
            REQUIRE(returnedState == particle.getState());
        }

        THEN("clients listening for removed objects are notified")
        {
            REQUIRE(listOneCallbackHasBeenCalled);
            REQUIRE(listOneCbId == particle.getId());

            REQUIRE(listTwoCallbackHasBeenCalled);
            REQUIRE(listTwoCbId == particle.getId());
        }
    }

    WHEN("Particle does not exist in project state")
    {
        Particle particleNotInState(source);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(listOne.removeObject(particleNotInState),
                              ObjectNotFound);
        }
    }

    WHEN("An Event in project state relies on this Particle")
    {
        state.appendChild(StateHelpers::createEventState(juce::Uuid(),
                                                         1,
                                                         particle.getId(),
                                                         juce::Uuid()),
                          nullptr);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(listOne.removeObject(particle), ObjectInUse);
        }
    }

    WHEN("Other entities are removed from the project state")
    {
        listOneCallbackHasBeenCalled = false;
        listTwoCallbackHasBeenCalled = false;
        state.removeChild(eventState, nullptr);

        THEN("clients listening for removed particles are not notified")
        {
            REQUIRE_FALSE(listOneCallbackHasBeenCalled);
            REQUIRE_FALSE(listTwoCallbackHasBeenCalled);
        }
    }
}

SCENARIO("ParticleList: onObjectUpdated")
{
    juce::ValueTree state(IDs::PROJECT_STATE);

    auto eventState = StateHelpers::createEventState(juce::Uuid());
    state.appendChild(eventState, nullptr);

    juce::Uuid sourceId;
    state.appendChild(StateHelpers::createSourceState(sourceId), nullptr);

    state.appendChild(StateHelpers::createParticleState(juce::Uuid(), sourceId),
                      nullptr);

    ProjectState projectState(state);

    auto particleList = projectState.getParticleList();

    REQUIRE(particleList.getObjects().size() == 1);

    auto particle = particleList.getObjects()[0];

    bool callbackHasBeenCalled = false;
    juce::Uuid returnedParticleId;
    juce::Identifier propChanged;
    particleList.onObjectUpdated =
        [&callbackHasBeenCalled,
         &propChanged,
         &returnedParticleId](Particle particle, juce::Identifier property) {
            callbackHasBeenCalled = true;
            propChanged = property;
            returnedParticleId = particle.getId();
        };

    WHEN("A particle is updated")
    {
        particle.setEnd(0.5);

        THEN("callback is called")
        {
            REQUIRE(callbackHasBeenCalled == true);
            REQUIRE(propChanged == IDs::end);
            REQUIRE(returnedParticleId == particle.getId());
        }
    }

    WHEN("An entity is updated that is not a particle")
    {
        eventState.setProperty(IDs::onset, 123, nullptr);

        THEN("callback is not called")
        {
            REQUIRE_FALSE(callbackHasBeenCalled);
        }
    }
}
