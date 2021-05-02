#include "ProjectState.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>
#include <stdexcept>

SCENARIO("EventList: all events: getObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid p1Id;
    juce::Uuid p2Id;
    juce::Uuid s1Id;
    juce::Uuid s2Id;
    juce::Uuid f1Id;
    juce::Uuid f2Id;
    state.appendChild(sh::createSourceState(s1Id), nullptr);
    state.appendChild(sh::createSourceState(s2Id), nullptr);
    state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);
    state.appendChild(sh::createParticleState(p2Id, s2Id), nullptr);
    state.appendChild(sh::createFigureState(f1Id), nullptr);
    state.appendChild(sh::createFigureState(f2Id), nullptr);

    WHEN("state has mixed entities but no events")
    {
        ProjectState projectState(state);
        auto eventList = projectState.getEventList();

        THEN("returns no events")
        {
            REQUIRE(eventList.getObjects().size() == 0);
        }
    }

    WHEN("state has mixed entities including events")
    {
        juce::Uuid e1Id;
        juce::Uuid e2Id;
        state.appendChild(sh::createEventState(e1Id, 1, p1Id, f1Id), nullptr);
        state.appendChild(sh::createEventState(e2Id, 2, p2Id, f2Id), nullptr);

        ProjectState projectState(state);
        auto eventList = projectState.getEventList();

        THEN("returns the events")
        {
            auto objects = eventList.getObjects();
            REQUIRE(objects.size() == 2);
            REQUIRE(objects[0].getId() == e1Id);
            REQUIRE(objects[1].getId() == e2Id);
        }
    }
}

SCENARIO("EventList: events for specific figure only: getObjects")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid p1Id;
    juce::Uuid p2Id;
    juce::Uuid s1Id;
    juce::Uuid s2Id;
    juce::Uuid f1Id;
    juce::Uuid f2Id;
    state.appendChild(sh::createSourceState(s1Id), nullptr);
    state.appendChild(sh::createSourceState(s2Id), nullptr);
    state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);
    state.appendChild(sh::createParticleState(p2Id, s2Id), nullptr);

    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);
    state.appendChild(sh::createFigureState(f2Id), nullptr);

    juce::Uuid e1Id;
    juce::Uuid e2Id;
    state.appendChild(sh::createEventState(e1Id, 1, p1Id, f1Id), nullptr);
    state.appendChild(sh::createEventState(e2Id, 2, p2Id, f2Id), nullptr);

    ProjectState projectState(state);
    Figure figure(figureState);
    auto eventList = projectState.getEventList(figure);

    THEN("only returns events related to this figure")
    {
        auto objects = eventList.getObjects();
        REQUIRE(objects.size() == 1);
        REQUIRE(objects[0].getId() == e1Id);
        REQUIRE(objects[0].getFigureId() == figure.getId());
    }
}

SCENARIO("EventList: all events: getIndex")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid s1Id;
    juce::Uuid p1Id;
    juce::Uuid f1Id;

    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);

    state.appendChild(sh::createFigureState(f1Id), nullptr);

    state.appendChild(sh::createEventState(juce::Uuid(), 1, p1Id, f1Id),
                      nullptr);
    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f1Id),
                      nullptr);
    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f1Id),
                      nullptr);

    ProjectState projectState(state);
    auto list = projectState.getEventList();

    REQUIRE(list.getObjects().size() == 3);

    WHEN("Event exists in the list")
    {
        auto eventToFind = list.getObjects()[1];

        THEN("returns the index")
        {
            REQUIRE(list.getIndex(eventToFind) == 1);
        }
    }

    WHEN("Particle cannot be found in list")
    {
        Source source(sourceState);
        Particle particle(source);
        Figure figure;
        Event orphanEvent(figure, particle, 1);

        THEN("returns -1")
        {
            REQUIRE(list.getIndex(orphanEvent) == -1);
        }
    }
}

SCENARIO("EventList: events for specific figure only: getIndex")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);
    juce::Uuid s1Id;
    juce::Uuid p1Id;
    juce::Uuid f1Id;
    juce::Uuid f2Id;

    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    state.appendChild(sh::createParticleState(p1Id, s1Id), nullptr);

    auto configuredFigureState = sh::createFigureState(f1Id);
    state.appendChild(configuredFigureState, nullptr);
    state.appendChild(sh::createFigureState(f2Id), nullptr);

    state.appendChild(sh::createEventState(juce::Uuid(), 1, p1Id, f1Id),
                      nullptr);
    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f1Id),
                      nullptr);
    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f1Id),
                      nullptr);

    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f2Id),
                      nullptr);
    state.appendChild(sh::createEventState(juce::Uuid(), 2, p1Id, f2Id),
                      nullptr);

    ProjectState projectState(state);
    Figure configuredFigure(configuredFigureState);
    auto list = projectState.getEventList(configuredFigure);

    REQUIRE(list.getObjects().size() == 3);

    WHEN("Event exists in the list")
    {
        auto eventToFind = list.getObjects()[1];

        THEN("returns the index")
        {
            REQUIRE(list.getIndex(eventToFind) == 1);
        }
    }

    WHEN("Event cannot be found in list")
    {
        Source source(sourceState);
        Particle particle(source);
        Figure figure;
        Event orphanEvent(figure, particle, 1);

        THEN("returns -1")
        {
            REQUIRE(list.getIndex(orphanEvent) == -1);
        }
    }
}

SCENARIO("EventList: all events: addObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    juce::Uuid s1Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    juce::Uuid p1Id;
    auto particleState = sh::createParticleState(p1Id, s1Id);
    state.appendChild(particleState, nullptr);

    juce::Uuid f1Id;
    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);

    juce::Uuid e1Id;
    auto eventState = sh::createEventState(e1Id, 1, p1Id, f1Id);
    state.appendChild(eventState, nullptr);

    ProjectState projectState(state);

    auto list1 = projectState.getEventList();
    auto list2 = projectState.getEventList();

    REQUIRE(list1.getObjects().size() == 1);
    REQUIRE(list2.getObjects().size() == 1);

    SECTION("Validating")
    {
        WHEN("Event's figure is not in project state")
        {
            Source source(sourceState);
            Figure invalidFigure;
            Particle particle(particleState, source);
            Event event(invalidFigure, particle, 1);

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(list1.addObject(event),
                                  ObjectDependencyNotFound);
            }
        }

        WHEN("Event's particle is not in project state")
        {
            Source source(sourceState);
            Particle invalidParticle(source);
            Figure figure(figureState);
            Event event(figure, invalidParticle, 1);

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(list1.addObject(event),
                                  ObjectDependencyNotFound);
            }
        }

        WHEN("Event already exists in project state")
        {
            Source source(sourceState);
            Particle particle(particleState, source);
            Figure figure(figureState);
            Event event(eventState, figure, particle);

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(list1.addObject(event), ObjectAlreadyExists);
            }
        }
    }

    SECTION("Adding a valid event")
    {
        Source source(sourceState);
        Particle particle(particleState, source);
        Figure figure(figureState);
        Event event(figure, particle, 1);

        bool l1CbCalled = false;
        juce::Uuid l1AddedEventId;
        list1.onObjectAdded = [&l1CbCalled, &l1AddedEventId](Event eventAdded) {
            l1CbCalled = true;
            l1AddedEventId = eventAdded.getId();
        };

        bool l2CbCalled = false;
        juce::Uuid l2AddedEventId;
        list2.onObjectAdded = [&l2CbCalled, &l2AddedEventId](Event eventAdded) {
            l2CbCalled = true;
            l2AddedEventId = eventAdded.getId();
        };

        auto returnedObject = list1.addObject(event);

        THEN("eventList objects increases by one")
        {
            REQUIRE(list1.getObjects().size() == 2);
            REQUIRE(list2.getObjects().size() == 2);
        }

        THEN("returned Event's state reflects having been added")
        {
            auto returnedState = returnedObject.getState();
            REQUIRE(returnedState.getParent().hasType(IDs::PROJECT_STATE));
            REQUIRE(returnedState == event.getState());
        }

        THEN("clients listening for added events are notified")
        {
            REQUIRE(l1CbCalled);
            REQUIRE(l1AddedEventId == event.getId());
            REQUIRE(l2CbCalled);
            REQUIRE(l2AddedEventId == event.getId());
        }

        WHEN("other entities are added to state")
        {
            l1CbCalled = false;
            l2CbCalled = false;

            state.appendChild(sh::createFigureState(), nullptr);

            THEN("clients listening for added events are not notified")
            {
                REQUIRE_FALSE(l1CbCalled);
                REQUIRE_FALSE(l2CbCalled);
            }
        }
    }
}

SCENARIO("EventList: events for specific figure only: addObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    juce::Uuid s1Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    juce::Uuid p1Id;
    auto particleState = sh::createParticleState(p1Id, s1Id);
    state.appendChild(particleState, nullptr);

    juce::Uuid f1Id;
    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);

    juce::Uuid f2Id;
    auto anotherFigureState = sh::createFigureState(f2Id);
    state.appendChild(anotherFigureState, nullptr);

    ProjectState projectState(state);

    Source source(sourceState);
    Particle particle(particleState, source);
    Figure figure(figureState);
    Figure anotherFigure(anotherFigureState);

    auto configuredList = projectState.getEventList(figure);
    bool configListCbCalled = false;
    configuredList.onObjectAdded = [&configListCbCalled](Event) {
        configListCbCalled = true;
    };

    auto configuredListWithAnother = projectState.getEventList(anotherFigure);
    bool configListAnotherCbCalled = false;
    configuredListWithAnother.onObjectAdded =
        [&configListAnotherCbCalled](Event) {
            configListAnotherCbCalled = true;
        };

    auto nonConfiguredList = projectState.getEventList();
    bool noConfigListCbCalled = false;
    nonConfiguredList.onObjectAdded = [&noConfigListCbCalled](Event) {
        noConfigListCbCalled = true;
    };

    REQUIRE(configuredList.getObjects().size() == 0);
    REQUIRE(configuredListWithAnother.getObjects().size() == 0);
    REQUIRE(nonConfiguredList.getObjects().size() == 0);

    WHEN("The event's figure does not match the figure the EventList has been "
         "configured with")
    {
        Event event(anotherFigure, particle, 1);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(configuredList.addObject(event),
                              EventFigureInvalidForEventList);
        }
    }

    WHEN("The event's figure matches figure the EventList has been "
         "configured with")
    {
        Event event(figure, particle, 1);
        configuredList.addObject(event);

        SECTION("list with this config")
        {
            THEN("objects increases by one")
            {
                REQUIRE(configuredList.getObjects().size() == 1);
            }

            THEN("clients listening for added events are notified")
            {
                REQUIRE(configListCbCalled);
            }
        }

        SECTION("list with another config")
        {
            THEN("objects does not increase")
            {
                REQUIRE(configuredListWithAnother.getObjects().size() == 0);
            }

            THEN("clients listening for added events are not notified")
            {
                REQUIRE_FALSE(configListAnotherCbCalled);
            }
        }

        SECTION("list with no config (all events)")
        {
            THEN("objects increases by one")
            {
                REQUIRE(nonConfiguredList.getObjects().size() == 1);
            }

            THEN("clients listening for added events are notified")
            {
                REQUIRE(noConfigListCbCalled);
            }
        }
    }
}

SCENARIO("EventList: all events: removeObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    juce::Uuid s1Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    juce::Uuid p1Id;
    auto particleState = sh::createParticleState(p1Id, s1Id);
    state.appendChild(particleState, nullptr);

    juce::Uuid f1Id;
    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);

    juce::Uuid f2Id;
    auto anotherFigureState = sh::createFigureState(f2Id);
    state.appendChild(anotherFigureState, nullptr);

    juce::Uuid e1Id;
    auto eventState = sh::createEventState(e1Id, 1, p1Id, f1Id);
    state.appendChild(eventState, nullptr);

    ProjectState projectState(state);

    auto list1 = projectState.getEventList();
    bool list1CbCalled = false;
    juce::Uuid list1CbId;
    list1.onObjectRemoved = [&list1CbCalled, &list1CbId](Event returnedEvent) {
        list1CbCalled = true;
        list1CbId = returnedEvent.getId();
    };

    auto list2 = projectState.getEventList();
    bool list2CbCalled = false;
    juce::Uuid list2CbId;
    list2.onObjectRemoved = [&list2CbCalled, &list2CbId](Event returnedEvent) {
        list2CbCalled = true;
        list2CbId = returnedEvent.getId();
    };

    REQUIRE(list1.getObjects().size() == 1);
    REQUIRE(list2.getObjects().size() == 1);

    WHEN("Event does not exist in project state")
    {
        Source source(sourceState);
        Particle particle(particleState, source);
        Figure figure(figureState);
        Event event(figure, particle, 1);

        THEN("throws error")
        {
            REQUIRE_THROWS_AS(list1.removeObject(event), ObjectNotFound);
        }
    }

    WHEN("Event does exist in project state")
    {
        Source source(sourceState);
        Particle particle(particleState, source);
        Figure figure(figureState);
        Event event(eventState, figure, particle);

        auto returnedEvent = list1.removeObject(event);

        THEN("event is removed from list of objects")
        {
            REQUIRE(list1.getObjects().size() == 0);
            REQUIRE(list2.getObjects().size() == 0);
        }

        THEN("returned Event's state reflects having been removed")
        {
            auto returnedState = returnedEvent.getState();
            REQUIRE_FALSE(returnedState.getParent().isValid());
            REQUIRE(returnedState == event.getState());
        }

        THEN("clients listening for removed objects are notified")
        {
            REQUIRE(list1CbCalled);
            REQUIRE(list1CbId == event.getId());

            REQUIRE(list2CbCalled);
            REQUIRE(list2CbId == event.getId());
        }
    }

    WHEN("other entities are removed from the project state")
    {
        state.removeChild(anotherFigureState, nullptr);

        THEN("clients listening for removed events are not notified")
        {
            REQUIRE_FALSE(list1CbCalled);
            REQUIRE_FALSE(list2CbCalled);
        }
    }
}

SCENARIO("EventList: events for specific figure only: removeObject")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    juce::Uuid s1Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    juce::Uuid p1Id;
    auto particleState = sh::createParticleState(p1Id, s1Id);
    state.appendChild(particleState, nullptr);

    juce::Uuid f1Id;
    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);

    juce::Uuid f2Id;
    auto anotherFigureState = sh::createFigureState(f2Id);
    state.appendChild(anotherFigureState, nullptr);

    juce::Uuid e1Id;
    auto eventState = sh::createEventState(e1Id, 1, p1Id, f1Id);
    state.appendChild(eventState, nullptr);

    juce::Uuid e2Id;
    auto anotherEventState = sh::createEventState(e2Id, 1, p1Id, f2Id);
    state.appendChild(anotherEventState, nullptr);

    ProjectState projectState(state);

    Source source(sourceState);
    Particle particle(particleState, source);
    Figure figure(figureState);
    Figure anotherFigure(anotherFigureState);
    Event event(eventState, figure, particle);
    Event anotherEvent(anotherEventState, anotherFigure, particle);

    auto configuredList = projectState.getEventList(figure);
    bool configListCbCalled = false;
    configuredList.onObjectRemoved = [&configListCbCalled](Event) {
        configListCbCalled = true;
    };

    auto configuredListWithAnother = projectState.getEventList(anotherFigure);
    bool configListAnotherCbCalled = false;
    configuredListWithAnother.onObjectRemoved =
        [&configListAnotherCbCalled](Event) {
            configListAnotherCbCalled = true;
        };

    auto nonConfiguredList = projectState.getEventList();
    bool noConfigListCbCalled = false;
    nonConfiguredList.onObjectRemoved = [&noConfigListCbCalled](Event) {
        noConfigListCbCalled = true;
    };

    REQUIRE(configuredList.getObjects().size() == 1);
    REQUIRE(configuredListWithAnother.getObjects().size() == 1);
    REQUIRE(nonConfiguredList.getObjects().size() == 2);

    WHEN("Event's figure does not match the figure the EventList has been "
         "configured with")
    {
        THEN("throws error")
        {
            REQUIRE_THROWS_AS(configuredList.removeObject(anotherEvent),
                              EventFigureInvalidForEventList);
        }
    }

    WHEN("Event's figure does match the figure the EventList has been "
         "configured with")
    {
        configuredList.removeObject(event);

        SECTION("list with this config")
        {
            THEN("objects decreases by one")
            {
                REQUIRE(configuredList.getObjects().size() == 0);
            }

            THEN("clients listening for added events are notified")
            {
                REQUIRE(configListCbCalled);
            }
        }

        SECTION("list with another config")
        {
            THEN("objects does not decrease")
            {
                REQUIRE(configuredListWithAnother.getObjects().size() == 1);
            }

            THEN("clients listening for added events are not notified")
            {
                REQUIRE_FALSE(configListAnotherCbCalled);
            }
        }

        SECTION("list with no config (all events)")
        {
            THEN("objects decreases by one")
            {
                REQUIRE(nonConfiguredList.getObjects().size() == 1);
            }

            THEN("clients listening for added events are notified")
            {
                REQUIRE(noConfigListCbCalled);
            }
        }
    }
}

SCENARIO("EventList: onObjectUpdated")
{
    using sh = StateHelpers;

    juce::ValueTree state(IDs::PROJECT_STATE);

    juce::Uuid s1Id;
    auto sourceState = sh::createSourceState(s1Id);
    state.appendChild(sourceState, nullptr);

    juce::Uuid p1Id;
    auto particleState = sh::createParticleState(p1Id, s1Id);
    state.appendChild(particleState, nullptr);

    juce::Uuid f1Id;
    auto figureState = sh::createFigureState(f1Id);
    state.appendChild(figureState, nullptr);

    juce::Uuid f2Id;
    auto anotherFigureState = sh::createFigureState(f2Id);
    state.appendChild(anotherFigureState, nullptr);

    juce::Uuid f3Id;
    auto unusedFigureState = sh::createFigureState(f3Id);
    state.appendChild(unusedFigureState, nullptr);

    juce::Uuid e1Id;
    auto eventState = sh::createEventState(e1Id, 1, p1Id, f1Id);
    state.appendChild(eventState, nullptr);

    juce::Uuid e2Id;
    auto anotherEventState = sh::createEventState(e2Id, 1, p1Id, f2Id);
    state.appendChild(anotherEventState, nullptr);

    juce::Uuid e3Id;
    auto unusedEventState = sh::createEventState(e3Id, 1, p1Id, f3Id);
    state.appendChild(unusedEventState, nullptr);

    ProjectState projectState(state);

    Source source(sourceState);
    Particle particle(particleState, source);
    Figure figure(figureState);
    Figure anotherFigure(anotherFigureState);
    Figure unusedFigure(unusedFigureState);
    Event event(eventState, figure, particle);
    Event anotherEvent(anotherEventState, anotherFigure, particle);
    Event unusedEvent(unusedEventState, unusedFigure, particle);

    auto configuredList = projectState.getEventList(figure);
    bool configListCbCalled = false;
    configuredList.onObjectUpdated = [&configListCbCalled](Event,
                                                           juce::Identifier) {
        configListCbCalled = true;
    };

    auto configuredListWithAnother = projectState.getEventList(anotherFigure);
    bool configListAnotherCbCalled = false;
    configuredListWithAnother.onObjectUpdated =
        [&configListAnotherCbCalled](Event, juce::Identifier) {
            configListAnotherCbCalled = true;
        };

    auto nonConfiguredList = projectState.getEventList();
    bool noConfigListCbCalled = false;
    juce::Uuid returnedEventId;
    juce::Identifier propChanged;
    nonConfiguredList.onObjectUpdated =
        [&noConfigListCbCalled,
         &returnedEventId,
         &propChanged](Event event, juce::Identifier property) {
            noConfigListCbCalled = true;
            returnedEventId = event.getId();
            propChanged = property;
        };

    REQUIRE(configuredList.getObjects().size() == 1);
    REQUIRE(configuredListWithAnother.getObjects().size() == 1);
    REQUIRE(nonConfiguredList.getObjects().size() == 3);

    WHEN("An event is updated")
    {
        SECTION("event belongs to figure for which a list is configured")
        {
            event.setOnset(2);

            THEN("configured list callback is called")
            {
                REQUIRE(configListCbCalled);
            }

            THEN("another configured list callback is not called")
            {
                REQUIRE_FALSE(configListAnotherCbCalled);
            }

            THEN("unconfigured list (all events) callback is called")
            {
                REQUIRE(noConfigListCbCalled);
                REQUIRE(propChanged == IDs::onset);
                REQUIRE(returnedEventId == event.getId());
            }
        }

        SECTION("event belongs to figure for which another list is confgured")
        {
            anotherEvent.setOnset(2);

            THEN("configured list callback is not called")
            {
                REQUIRE_FALSE(configListCbCalled);
            }

            THEN("another configured list callback is called")
            {
                REQUIRE(configListAnotherCbCalled);
            }

            THEN("unconfigured list (all events) callback is called")
            {
                REQUIRE(noConfigListCbCalled);
                REQUIRE(propChanged == IDs::onset);
                REQUIRE(returnedEventId == anotherEvent.getId());
            }
        }

        SECTION("event belongs to figure for which no lists are configured")
        {
            unusedEvent.setOnset(2);

            THEN("neither configured lists callbacks are called")
            {
                REQUIRE_FALSE(configListCbCalled);
                REQUIRE_FALSE(configListAnotherCbCalled);
            }

            THEN("unconfigured list callback is called")
            {
                REQUIRE(noConfigListCbCalled);
                REQUIRE(propChanged == IDs::onset);
                REQUIRE(returnedEventId == unusedEvent.getId());
            }
        }
    }

    WHEN("An entity is updated that is not an event")
    {
        particleState.setProperty(IDs::end, 0.4, nullptr);

        THEN("No list callbacks are called")
        {
            REQUIRE_FALSE(configListCbCalled);
            REQUIRE_FALSE(configListAnotherCbCalled);
            REQUIRE_FALSE(noConfigListCbCalled);
        }
    }
}
