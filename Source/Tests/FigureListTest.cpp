#include "ProjectState.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("FigureList: getObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createSourceState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);
    state.appendChild(sh::createEventState(), nullptr);

    WHEN("state has mixed entities but no figures")
    {
        ProjectState projectState(state);
        auto list = projectState.getFigureList();

        THEN("returns no objects")
        {
            REQUIRE(list.getObjects().size() == 0);
        }
    }

    WHEN("state has mixed entities including figures")
    {
        juce::Uuid f1Id;
        juce::Uuid f2Id;
        state.appendChild(sh::createFigureState(f1Id), nullptr);
        state.appendChild(sh::createFigureState(f2Id), nullptr);

        ProjectState projectState(state);
        auto list = projectState.getFigureList();

        THEN("returns the figures")
        {
            auto objects = list.getObjects();
            REQUIRE(objects.size() == 2);
            REQUIRE(objects[0].getId() == f1Id);
            REQUIRE(objects[1].getId() == f2Id);
        }
    }
}

SCENARIO("FigureList: getIndex")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);
    state.appendChild(sh::createFigureState(), nullptr);

    ProjectState projectState(state);
    auto list = projectState.getFigureList();

    REQUIRE(list.getObjects().size() == 3);

    WHEN("Figure exists in the list")
    {
        auto figureToFind = list.getObjects()[1];

        THEN("returns the index")
        {
            REQUIRE(list.getIndex(figureToFind) == 1);
        }
    }

    WHEN("Figure cannot be found in list")
    {
        Figure orphanParticle;

        THEN("returns -1")
        {
            REQUIRE(list.getIndex(orphanParticle) == -1);
        }
    }
}

SCENARIO("FigureList: addObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    auto figureState = sh::createFigureState();
    state.appendChild(figureState, nullptr);

    ProjectState projectState(state);

    auto list1 = projectState.getFigureList();
    bool list1CbCalled = false;
    juce::Uuid list1AddedFigureId;
    list1.onObjectAdded = [&list1CbCalled, &list1AddedFigureId](Figure f) {
        list1CbCalled = true;
        list1AddedFigureId = f.getId();
    };

    auto list2 = projectState.getFigureList();
    bool list2CbCalled = false;
    juce::Uuid list2AddedFigureId;
    list2.onObjectAdded = [&list2CbCalled, &list2AddedFigureId](Figure f) {
        list2CbCalled = true;
        list2AddedFigureId = f.getId();
    };

    REQUIRE(list1.getObjects().size() == 1);
    REQUIRE(list2.getObjects().size() == 1);

    WHEN("figure already exists in project state")
    {
        Figure figure(figureState);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.addObject(figure), ObjectAlreadyExists);
        }
    }

    WHEN("figure does not exist in project state")
    {
        Figure figure;
        auto returnedFigure = list1.addObject(figure);

        THEN("lists objects increase by one")
        {
            REQUIRE(list1.getObjects().size() == 2);
            REQUIRE(list2.getObjects().size() == 2);
        }

        THEN("Returned figure's state reflect having been added")
        {
            auto returnedState = returnedFigure.getState();
            REQUIRE(returnedState.getParent().hasType(IDs::PROJECT_STATE));
            REQUIRE(returnedState == figure.getState());
        }

        THEN("clients listening for added figures are notified")
        {
            REQUIRE(list1CbCalled);
            REQUIRE(list1AddedFigureId == figure.getId());
            REQUIRE(list2CbCalled);
            REQUIRE(list2AddedFigureId == figure.getId());
        }
    }

    WHEN("other entities are added to the state")
    {
        state.appendChild(sh::createSourceState(), nullptr);

        THEN("clients listening for added figures are not notified")
        {
            REQUIRE_FALSE(list1CbCalled);
            REQUIRE_FALSE(list2CbCalled);
        }
    }
}

SCENARIO("FigureList: removeObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    auto figState1 = sh::createFigureState();
    auto figState2 = sh::createFigureState();
    state.appendChild(figState1, nullptr);
    state.appendChild(figState2, nullptr);

    auto eventState = sh::createEventState(juce::Uuid(),
                                           1,
                                           juce::Uuid(),
                                           juce::Uuid(figState1[IDs::id]));
    state.appendChild(eventState, nullptr);

    Figure figure1(figState1);
    Figure figure2(figState2);

    ProjectState projectState(state);

    auto list1 = projectState.getFigureList();
    bool list1CbCalled = false;
    juce::Uuid list1RemovedId;
    list1.onObjectRemoved = [&list1CbCalled, &list1RemovedId](Figure f) {
        list1CbCalled = true;
        list1RemovedId = f.getId();
    };

    auto list2 = projectState.getFigureList();
    bool list2CbCalled = false;
    juce::Uuid list2RemovedId;
    list2.onObjectRemoved = [&list2CbCalled, &list2RemovedId](Figure f) {
        list2CbCalled = true;
        list2RemovedId = f.getId();
    };

    REQUIRE(list1.getObjects().size() == 2);
    REQUIRE(list2.getObjects().size() == 2);

    WHEN("Figure does not exist in project state")
    {
        Figure figureNotInState;
        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.removeObject(figureNotInState),
                              ObjectNotFound);
        }
    }

    WHEN("An event relies on the figure to be removed")
    {
        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.removeObject(figure1), ObjectInUse);
        }
    }

    WHEN("The figure is ok to remove")
    {
        auto returnedFigure = list1.removeObject(figure2);

        THEN("figure is removed from lists objects")
        {
            REQUIRE(list1.getObjects().size() == 1);
            REQUIRE(list2.getObjects().size() == 1);
        }

        THEN("returned Figure's state reflects having been removed")
        {
            auto returnedState = returnedFigure.getState();
            REQUIRE_FALSE(returnedState.getParent().isValid());
            REQUIRE(returnedState == figure2.getState());
        }

        THEN("clients listening for removed objects are notified")
        {
            REQUIRE(list1CbCalled);
            REQUIRE(list1RemovedId == figure2.getId());

            REQUIRE(list2CbCalled);
            REQUIRE(list2RemovedId == figure2.getId());
        }
    }

    WHEN("other entities are removed from the project state")
    {
        state.removeChild(eventState, nullptr);

        THEN("clients listening for removed figures are not notified")
        {
            REQUIRE_FALSE(list1CbCalled);
            REQUIRE_FALSE(list2CbCalled);
        }
    }
}
