#include "SourceList.h"

#include "ErrorTypes.h"
#include "ProjectState.h"
#include "StateHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("SourceList: getObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    state.appendChild(sh::createParticleState(), nullptr);
    state.appendChild(sh::createParticleState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);

    WHEN("state has mixed entities but no sources")
    {
        ProjectState projectState(state);
        auto sourceList = projectState.getSourceList();

        THEN("returns no sources")
        {
            REQUIRE(sourceList.getObjects().size() == 0);
        }
    }

    WHEN("state has mixed entities including sources")
    {
        state.appendChild(sh::createSourceState(), nullptr);
        state.appendChild(sh::createSourceState(), nullptr);

        ProjectState projectState(state);
        auto sourceList = projectState.getSourceList();

        THEN("returns the sources")
        {
            REQUIRE(sourceList.getObjects().size() == 2);
        }
    }
}

SCENARIO("SourceList:: getIndex")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createSourceState(), nullptr);

    ProjectState projectState(state);
    auto list = projectState.getSourceList();

    REQUIRE(list.getObjects().size() == 3);

    WHEN("Source exists in the list")
    {
        auto sourceToFind = list.getObjects()[1];

        THEN("returns the index")
        {
            REQUIRE(list.getIndex(sourceToFind) == 1);
        }
    }

    WHEN("Source cannot be found in list")
    {
        Source orphanSource(sh::createSourceState());

        THEN("returns -1")
        {
            REQUIRE(list.getIndex(orphanSource) == -1);
        }
    }
}

SCENARIO("SourceList: addObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    auto existingSourceState = sh::createSourceState();
    state.appendChild(existingSourceState, nullptr);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createParticleState(), nullptr);
    state.appendChild(sh::createParticleState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);

    ProjectState projectState(state);
    auto list1 = projectState.getSourceList();
    bool list1CallbackHasBeenCalled;
    juce::Uuid list1CbSourceId;
    list1.onObjectAdded = [&list1CallbackHasBeenCalled,
                           &list1CbSourceId](Source s) {
        list1CallbackHasBeenCalled = true;
        list1CbSourceId = s.getId();
    };

    auto list2 = projectState.getSourceList();
    bool list2CallbackHasBeenCalled;
    juce::Uuid list2CbSourceId;
    list2.onObjectAdded = [&list2CallbackHasBeenCalled,
                           &list2CbSourceId](Source s) {
        list2CallbackHasBeenCalled = true;
        list2CbSourceId = s.getId();
    };

    REQUIRE(list1.getObjects().size() == 2);
    REQUIRE(list2.getObjects().size() == 2);

    WHEN("Source does not already exist in project state")
    {
        auto filepath =
            juce::String(AUDIO_ASSETS_DIR) + "/whitenoise-2000ms-copy.wav";
        juce::File newFile(filepath);
        Source newSource(newFile);
        auto returnedSource = list1.addObject(newSource);

        THEN("all lists' objects increases by one")
        {
            REQUIRE(list1.getObjects().size() == 3);
            REQUIRE(list2.getObjects().size() == 3);
        }

        THEN("Returned Source's state reflects having been added")
        {
            auto returnedState = returnedSource.getState();
            REQUIRE(returnedState.getParent().hasType(IDs::PROJECT_STATE));
            REQUIRE(returnedState == newSource.getState());
        }

        THEN("clients listening for added particles are notified")
        {
            REQUIRE(list1CallbackHasBeenCalled);
            REQUIRE(list1CbSourceId == newSource.getId());
            REQUIRE(list2CallbackHasBeenCalled);
            REQUIRE(list2CbSourceId == newSource.getId());
        }
    }

    WHEN("other entities are added to the state")
    {
        list1CallbackHasBeenCalled = false;
        list2CallbackHasBeenCalled = false;
        state.appendChild(sh::createEventState(), nullptr);

        THEN("clients listening to source lists are not notified")
        {
            REQUIRE_FALSE(list1CallbackHasBeenCalled);
            REQUIRE_FALSE(list2CallbackHasBeenCalled);
        }
    }

    WHEN("Source already exists in project state")
    {
        Source existingSource(existingSourceState);
        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.addObject(existingSource),
                              ObjectAlreadyExists);
        }
    }

    WHEN("File path for source already exists in project state")
    {
        juce::File newFile(existingSourceState[IDs::file_path]);
        Source sourceWithExistingFile(newFile);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.addObject(sourceWithExistingFile),
                              FileAlreadyExists);
        }
    }
}

SCENARIO("SourceList:: removeObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    auto eventState = sh::createEventState();
    juce::Uuid sourceIdInUse;
    auto sourceStateInUse = sh::createSourceState(sourceIdInUse);
    auto sourceStateNotInUse = sh::createSourceState();
    auto particleState = sh::createParticleState(juce::Uuid(), sourceIdInUse);

    state.appendChild(eventState, nullptr);
    state.appendChild(sourceStateInUse, nullptr);
    state.appendChild(sourceStateNotInUse, nullptr);
    state.appendChild(particleState, nullptr);

    ProjectState projectState(state);

    auto list1 = projectState.getSourceList();
    auto list2 = projectState.getSourceList();

    bool list1CallbackHasBeenCalled;
    juce::Uuid list1CbSourceId;
    list1.onObjectRemoved = [&list1CallbackHasBeenCalled,
                             &list1CbSourceId](Source addedSource) {
        list1CallbackHasBeenCalled = true;
        list1CbSourceId = addedSource.getId();
    };

    bool list2CallbackHasBeenCalled;
    juce::Uuid list2CbSourceId;
    list2.onObjectRemoved = [&list2CallbackHasBeenCalled,
                             &list2CbSourceId](Source addedSource) {
        list2CallbackHasBeenCalled = true;
        list2CbSourceId = addedSource.getId();
    };

    REQUIRE(list1.getObjects().size() == 2);
    REQUIRE(list2.getObjects().size() == 2);

    WHEN("Source does not exist in project state")
    {
        THEN("throws error")
        {
            Source orphanSource(sh::createSourceState());
            REQUIRE_THROWS_AS(list1.removeObject(orphanSource), ObjectNotFound);
        }
    }

    WHEN("Source is in use by a Particle in the project state")
    {
        THEN("throws error")
        {
            Source sourceToRemove(sourceStateInUse);
            REQUIRE_THROWS_AS(list1.removeObject(sourceToRemove), ObjectInUse);
        }
    }

    WHEN("Source can be removed")
    {
        Source sourceToRemove(sourceStateNotInUse);
        auto returnedSource = list1.removeObject(sourceToRemove);

        THEN("source is removed from list of objects")
        {
            REQUIRE(list1.getObjects().size() == 1);
            REQUIRE(list2.getObjects().size() == 1);
        }

        THEN("returned Source's state reflects having been removed")
        {
            auto returnedState = returnedSource.getState();
            REQUIRE_FALSE(returnedState.getParent().isValid());
            REQUIRE(returnedState == returnedSource.getState());
        }

        THEN("clients listening for removed objects are notified")
        {
            REQUIRE(list1CallbackHasBeenCalled);
            REQUIRE(list1CbSourceId == sourceToRemove.getId());

            REQUIRE(list2CallbackHasBeenCalled);
            REQUIRE(list2CbSourceId == sourceToRemove.getId());
        }
    }

    WHEN("Other entities are removed from the project state")
    {
        list1CallbackHasBeenCalled = false;
        list2CallbackHasBeenCalled = false;
        state.removeChild(eventState, nullptr);

        THEN("clients listening for removed sources are not notified")
        {
            REQUIRE_FALSE(list1CallbackHasBeenCalled);
            REQUIRE_FALSE(list2CallbackHasBeenCalled);
        }
    }
}
