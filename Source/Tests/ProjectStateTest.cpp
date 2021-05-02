#include "ProjectState.h"

#include "Identifiers.h"
#include "TestHelpers.h"

#include <catch2/catch.hpp>
#include <juce_core/juce_core.h>

/*
NB: Notes for this test file:

1. all files file.create() must have an accompanying file.delete()

2. only use CHECK... verions of assertions. REQUIRE... will prevent clean up by
halting the code process and thus files won't be cleaned up

3. use uniquely named file names to avoid one test case deleting a file the next
case is relying on because test cases run concurrently
*/

SCENARIO("Project state: save")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    WHEN("project state does not have a file")
    {
        // fill state
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());

        // NB: register listener after adding to state
        bool cbCalled = false;
        std::unique_ptr<ProjectState::Status> cbStatus;
        std::unique_ptr<ProjectState::Action> cbAction;
        projectState.onStatusChanged =
            [&cbCalled, &cbStatus, &cbAction](auto s, auto a) {
                cbCalled = true;
                cbStatus = std::make_unique<ProjectState::Status>(s);
                cbAction = std::make_unique<ProjectState::Action>(a);
            };

        // Initial values
        CHECK(projectState.getFile() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasFile);
        CHECK(projectState.getStatus().hasUnsavedChanges);

        AND_WHEN("no file is provided")
        {
            CHECK_THROWS_AS(projectState.save(), ProjectFileNotFound);
            CHECK(projectState.getFile() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasFile);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);
        }

        AND_WHEN("an invalid file is provided")
        {
            SECTION("File provided does not exist")
            {
                juce::File file;
                CHECK_THROWS_AS(projectState.save(file), InvalidFilePath);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }

            SECTION("File provided has a path to a directory")
            {
                juce::File file(filepath);
                CHECK_THROWS_AS(projectState.save(file), InvalidFilePath);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }

            SECTION("File provided is not xml")
            {
                filepath += FileHelpers::getTestFileName() + ".txt";
                juce::File file(filepath);
                file.create();

                CHECK_THROWS_AS(projectState.save(file), InvalidProjectFile);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }
        }

        AND_WHEN("a valid file is provided")
        {
            filepath += FileHelpers::getTestFileName() + ".xml";
            juce::File file(filepath);
            file.create();

            CHECK_FALSE(projectState.getStatus().hasFile);

            projectState.save(file);

            THEN("the provided file is stored in project state")
            {
                CHECK(*projectState.getFile() == file);
            }

            THEN("status hasFile is set to true")
            {
                CHECK(projectState.getStatus().hasFile);
            }

            THEN("hasUnsavedChanges is false")
            {
                CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            }

            THEN("callback is called")
            {
                CHECK(cbCalled);
                CHECK_FALSE(cbStatus->hasUnsavedChanges);
                CHECK(cbStatus->hasFile);
                CHECK(*cbAction == ProjectState::Action::SaveToNewFile);
            }

            file.deleteFile(); // clean up
        }
    }

    WHEN("project state does have a file")
    {
        auto originalFilepath =
            filepath + FileHelpers::getTestFileName() + ".xml";
        juce::File originalFile(originalFilepath);
        originalFile.create();

        // save by providing file so project state has file
        projectState.save(originalFile);

        // fill state
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());

        // NB: register listener after adding to state
        bool cbCalled = false;
        std::unique_ptr<ProjectState::Status> cbStatus;
        std::unique_ptr<ProjectState::Action> cbAction;
        projectState.onStatusChanged =
            [&cbCalled, &cbStatus, &cbAction](auto s, auto a) {
                cbCalled = true;
                cbStatus = std::make_unique<ProjectState::Status>(s);
                cbAction = std::make_unique<ProjectState::Action>(a);
            };

        // Initial values
        CHECK_FALSE(projectState.getFile() == nullptr);
        CHECK(projectState.getStatus().hasFile);
        CHECK(projectState.getStatus().hasUnsavedChanges);

        WHEN("An invalid file is provided")
        {
            juce::File newFile;

            THEN("throws exception and does not change object state")
            {
                CHECK_THROWS_AS(projectState.save(newFile), InvalidFilePath);
                CHECK(*projectState.getFile() == originalFile);
                CHECK(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }
        }

        WHEN("An valid file is provided")
        {
            auto newFilepath =
                filepath + FileHelpers::getTestFileName() + ".xml";
            juce::File newFile(newFilepath);
            newFile.create();

            projectState.save(newFile);

            THEN("the provided file is stored in project state")
            {
                CHECK(*projectState.getFile() == newFile);
            }

            THEN("status hasFile is set to true")
            {
                CHECK(projectState.getStatus().hasFile);
            }

            THEN("hasUnsavedChanges is false")
            {
                CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            }

            THEN("callback is called")
            {
                CHECK(cbCalled);
                CHECK_FALSE(cbStatus->hasUnsavedChanges);
                CHECK(cbStatus->hasFile);
                CHECK(*cbAction == ProjectState::Action::SaveToNewFile);
            }

            newFile.deleteFile(); // clean up
        }

        WHEN("no file is provided")
        {
            CHECK_FALSE(cbCalled);

            projectState.save();

            THEN("state is saved to the file stored in project state")
            {
                // NB: scope here is important as load() will affect the tests
                // below

                // empty state
                state.removeAllChildren(nullptr);
                CHECK(fl.getObjects().size() == 0);
                // load state
                projectState.load(originalFile);
                CHECK(fl.getObjects().size() == 1);
            }

            THEN("file stored in project state is unchanged")
            {
                CHECK(*projectState.getFile() == originalFile);
            }

            THEN("status hasFile is still true")
            {
                CHECK(projectState.getStatus().hasFile);
            }

            THEN("status hasUnsavedChanges is false")
            {
                CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            }

            THEN("callback is called")
            {
                CHECK(cbCalled);
                CHECK_FALSE(cbStatus->hasUnsavedChanges);
                CHECK(cbStatus->hasFile);
                CHECK(*cbAction == ProjectState::Action::SaveToExistingFile);
            }
        }

        originalFile.deleteFile(); // clean up
    }
}

SCENARIO("Project state: load")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    juce::File newFile(filepath + FileHelpers::getTestFileName() + ".xml");
    newFile.create();

    // populate file
    {
        // temp - empty - project state used to save to a file
        juce::ValueTree tempState(IDs::PROJECT_STATE);
        ProjectState tempProjectState(tempState);
        tempProjectState.save(newFile);
    }

    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    // fill state
    auto fl = projectState.getFigureList();
    fl.addObject(Figure());

    WHEN("project state does not have a file")
    {
        CHECK(state.getNumChildren() == 1);
        CHECK(projectState.getFile() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasFile);
        CHECK(projectState.getStatus().hasUnsavedChanges);

        // NB: register listener after set up
        bool cbCalled = false;
        std::unique_ptr<ProjectState::Status> cbStatus;
        std::unique_ptr<ProjectState::Action> cbAction;
        projectState.onStatusChanged =
            [&cbCalled, &cbStatus, &cbAction](auto s, auto a) {
                cbCalled = true;
                cbStatus = std::make_unique<ProjectState::Status>(s);
                cbAction = std::make_unique<ProjectState::Action>(a);
            };

        AND_WHEN("an invalid file is provided")
        {
            SECTION("File provided does not exist")
            {
                juce::File file;
                CHECK_THROWS_AS(projectState.load(file), InvalidFilePath);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }

            SECTION("File provided has a path to a directory")
            {
                juce::File file(filepath);
                CHECK_THROWS_AS(projectState.load(file), InvalidFilePath);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }

            SECTION("File provided is not xml")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".txt");
                file.create();

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file is empty")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file contains invalid xml")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();
                juce::FileOutputStream os(file);
                os.writeText("<foo>", false, false, nullptr); // no end tag
                os.flush();

                auto fileContent = file.createInputStream()->readString();
                CHECK(fileContent == "<foo>");

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file has a top level value tree of the wrong type")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();
                juce::FileOutputStream os(file);
                os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                             "\"?><foo></foo>",
                             false,
                             false,
                             nullptr);
                os.flush();

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file has top level value tree with props")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();
                juce::FileOutputStream os(file);
                os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                             "\"?><PROJECT_STATE foo=\"bar\" />",
                             false,
                             false,
                             nullptr);
                os.flush();

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file has invalid children (wrong type)")
            {
                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();
                juce::FileOutputStream os(file);
                os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                             "\"?><PROJECT_STATE><FOO></FOO></PROJECT_STATE>",
                             false,
                             false,
                             nullptr);
                os.flush();

                CHECK_THROWS_AS(projectState.load(file), InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getFile() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasFile);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);

                file.deleteFile(); // clean up
            }

            SECTION("The file has invalid representations of objects")
            {
                // Easiest way to check this is to include children with valid
                // types but no props. That ensures that something will throw an
                // error somewhere. There is no point checking all reasons that
                // there might be invalid representations of data as this is
                // just a repeat of the object and list tests. Just need to know
                // that before replacing the state with the one loaded from
                // file, that we have tried to create object lists for each
                // object type.

                juce::File file(filepath + FileHelpers::getTestFileName() +
                                ".xml");
                file.create();
                juce::FileOutputStream os(file);
                juce::String textContent =
                    "<?xml version=\" 1.0 \" encoding=\" UTF - 8 \"?>";

                SECTION("Event")
                {
                    textContent += "<PROJECT_STATE><EVENT/></PROJECT_STATE>";
                    os.writeText(textContent, false, false, nullptr);
                    os.flush();

                    CHECK_THROWS_AS(projectState.load(file),
                                    InvalidProjectFile);
                    CHECK(state.getNumChildren() == 1);
                    CHECK(projectState.getFile() == nullptr);
                    CHECK_FALSE(projectState.getStatus().hasFile);
                    CHECK(projectState.getStatus().hasUnsavedChanges);
                    CHECK_FALSE(cbCalled);
                    CHECK(cbStatus == nullptr);
                    CHECK(cbAction == nullptr);
                }

                SECTION("Figure")
                {
                    textContent += "<PROJECT_STATE><FIGURE/></PROJECT_STATE>";
                    os.writeText(textContent, false, false, nullptr);
                    os.flush();

                    CHECK_THROWS_AS(projectState.load(file),
                                    InvalidProjectFile);
                    CHECK(projectState.getFile() == nullptr);
                    CHECK_FALSE(projectState.getStatus().hasFile);
                    CHECK(projectState.getStatus().hasUnsavedChanges);
                    CHECK_FALSE(cbCalled);
                    CHECK(cbStatus == nullptr);
                    CHECK(cbAction == nullptr);
                }

                SECTION("Particle")
                {
                    textContent += "<PROJECT_STATE><PARTICLE/></PROJECT_STATE>";
                    os.writeText(textContent, false, false, nullptr);
                    os.flush();

                    CHECK_THROWS_AS(projectState.load(file),
                                    InvalidProjectFile);
                    CHECK(projectState.getFile() == nullptr);
                    CHECK_FALSE(projectState.getStatus().hasFile);
                    CHECK(projectState.getStatus().hasUnsavedChanges);
                    CHECK_FALSE(cbCalled);
                    CHECK(cbStatus == nullptr);
                    CHECK(cbAction == nullptr);
                }

                SECTION("Source")
                {
                    textContent += "<PROJECT_STATE><SOURCE/></PROJECT_STATE>";
                    os.writeText(textContent, false, false, nullptr);
                    os.flush();

                    CHECK_THROWS_AS(projectState.load(file),
                                    InvalidProjectFile);
                    CHECK(projectState.getFile() == nullptr);
                    CHECK_FALSE(projectState.getStatus().hasFile);
                    CHECK(projectState.getStatus().hasUnsavedChanges);
                    CHECK_FALSE(cbCalled);
                    CHECK(cbStatus == nullptr);
                    CHECK(cbAction == nullptr);
                }

                file.deleteFile(); // clean up
            }
        }

        AND_WHEN("a valid file is provided")
        {
            projectState.load(newFile);

            THEN("state should be changed")
            {
                CHECK(state.getNumChildren() == 0);
            }

            THEN("the provided file is stored in project state")
            {
                CHECK(*projectState.getFile() == newFile);
            }

            THEN("status hasFile is set to true")
            {
                CHECK(projectState.getStatus().hasFile);
            }

            THEN("hasUnsavedChanges is false")
            {
                CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            }

            THEN("callback is called")
            {
                CHECK(cbCalled);
                CHECK_FALSE(cbStatus->hasUnsavedChanges);
                CHECK(cbStatus->hasFile);
                CHECK(*cbAction == ProjectState::Action::LoadNewFile);
            }
        }
    }

    WHEN("project state does have a file")
    {
        juce::File originalFile(filepath + FileHelpers::getTestFileName() +
                                ".xml");
        originalFile.create();
        projectState.save(originalFile);

        fl.addObject(Figure());

        CHECK(state.getNumChildren() == 2);
        CHECK_FALSE(projectState.getFile() == nullptr);
        CHECK(*projectState.getFile() == originalFile);
        CHECK(projectState.getStatus().hasFile);
        CHECK(projectState.getStatus().hasUnsavedChanges);

        // NB: register listener after set up
        bool cbCalled = false;
        std::unique_ptr<ProjectState::Status> cbStatus;
        std::unique_ptr<ProjectState::Action> cbAction;
        projectState.onStatusChanged =
            [&cbCalled, &cbStatus, &cbAction](auto s, auto a) {
                cbCalled = true;
                cbStatus = std::make_unique<ProjectState::Status>(s);
                cbAction = std::make_unique<ProjectState::Action>(a);
            };

        AND_WHEN("an new invalid file is provided")
        {
            // only one test case here of the possible ones from above
            juce::File file;

            CHECK_THROWS_AS(projectState.load(file), InvalidFilePath);
            CHECK(state.getNumChildren() == 2);
            CHECK_FALSE(projectState.getFile() == nullptr);
            CHECK(*projectState.getFile() == originalFile);
            CHECK(projectState.getStatus().hasFile);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);
        }

        AND_WHEN("a new valid file is provided")
        {
            projectState.load(newFile);

            THEN("state is changed to what is in loaded file")
            {
                CHECK(state.getNumChildren() == 0);
            }

            THEN("provided file replaces file stored in project state")
            {
                CHECK_FALSE(projectState.getFile() == nullptr);
                CHECK(*projectState.getFile() == newFile);
            }

            THEN("status hasFile is still true")
            {
                CHECK(projectState.getStatus().hasFile);
            }

            THEN("hasUnsavedChanges is false")
            {
                CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            }

            THEN("callback is called")
            {
                CHECK(cbCalled);
                CHECK_FALSE(cbStatus->hasUnsavedChanges);
                CHECK(cbStatus->hasFile);
                CHECK(*cbAction == ProjectState::Action::LoadNewFile);
            }
        }

        originalFile.deleteFile(); // clean up
    }

    newFile.deleteFile(); // clean up
}

SCENARIO("Project state: Save and load scenario flows")
{
    // NB: Because tests are relying on a flow of events, all actions (method
    // calls) need to happen at the outer level, not in nested whens / thens

    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    filepath += FileHelpers::getTestFileName() + ".xml";
    juce::File file(filepath);
    file.create();

    // state stage 1: empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    // state stage 2: filled
    auto pathBase = juce::String(ASSETS_DIR) + "/audio";
    auto sourcePath = pathBase + "/whitenoise-2000ms.wav";
    juce::File sourceFile(sourcePath);

    auto sl = projectState.getSourceList();
    Source s(sourceFile);
    sl.addObject(s);

    auto pl = projectState.getParticleList();
    Particle p(s);
    pl.addObject(p);

    auto fl = projectState.getFigureList();
    Figure f;
    fl.addObject(f);

    auto el = projectState.getEventList();
    Event e(f, p, 0);
    el.addObject(e);

    CHECK(sl.getObjects().size() == 1);
    CHECK(pl.getObjects().size() == 1);
    CHECK(fl.getObjects().size() == 1);
    CHECK(el.getObjects().size() == 1);
    CHECK(state.getNumChildren() == 4);

    // state save to file
    projectState.save(file);

    // state stage 3: empty state
    state.removeAllChildren(nullptr);

    CHECK(sl.getObjects().size() == 0);
    CHECK(pl.getObjects().size() == 0);
    CHECK(fl.getObjects().size() == 0);
    CHECK(el.getObjects().size() == 0);
    CHECK(state.getNumChildren() == 0);

    // state load:
    projectState.load(file);

    WHEN("state is loaded")
    {
        THEN("state should be same as at stage 2")
        {
            CHECK(sl.getObjects().size() == 1);
            CHECK(pl.getObjects().size() == 1);
            CHECK(fl.getObjects().size() == 1);
            CHECK(el.getObjects().size() == 1);
            CHECK(state.getNumChildren() == 4);
        }
    }

    // state stage 4: change state
    el.removeObject(el.getObjects()[0]);
    fl.removeObject(fl.getObjects()[0]);

    CHECK(sl.getObjects().size() == 1);
    CHECK(pl.getObjects().size() == 1);
    CHECK(fl.getObjects().size() == 0);
    CHECK(el.getObjects().size() == 0);
    CHECK(state.getNumChildren() == 2);

    // overwrite save to file
    projectState.save();

    // state stage 5: empty state
    state.removeAllChildren(nullptr);

    CHECK(sl.getObjects().size() == 0);
    CHECK(pl.getObjects().size() == 0);
    CHECK(fl.getObjects().size() == 0);
    CHECK(el.getObjects().size() == 0);
    CHECK(state.getNumChildren() == 0);

    // state load:  assert state same as stage 4
    projectState.load(file);

    WHEN("state is loaded, having overwritten previous state (stage 2) on file")
    {
        THEN("state should be same as at stage 4")
        {
            CHECK(sl.getObjects().size() == 1);
            CHECK(pl.getObjects().size() == 1);
            CHECK(fl.getObjects().size() == 0);
            CHECK(el.getObjects().size() == 0);
            CHECK(state.getNumChildren() == 2);
        }
    }

    file.deleteFile(); // clean up
}

SCENARIO("Project state: Loading a file notifies list listeners")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    filepath += FileHelpers::getTestFileName() + ".xml";
    juce::File file(filepath);
    file.create();

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    auto pathBase = juce::String(ASSETS_DIR) + "/audio";
    auto sourcePath = pathBase + "/whitenoise-2000ms.wav";
    juce::File sourceFile(sourcePath);

    SECTION("onObjectAdded")
    {
        // state filled
        auto sl = projectState.getSourceList();
        Source s(sourceFile);
        sl.addObject(s);

        auto pl = projectState.getParticleList();
        Particle p(s);
        pl.addObject(p);

        auto fl = projectState.getFigureList();
        Figure f;
        fl.addObject(f);

        auto el = projectState.getEventList();
        Event e(f, p, 0);
        el.addObject(e);

        CHECK(sl.getObjects().size() == 1);
        CHECK(pl.getObjects().size() == 1);
        CHECK(fl.getObjects().size() == 1);
        CHECK(el.getObjects().size() == 1);
        CHECK(state.getNumChildren() == 4);

        // state save to file
        projectState.save(file);

        // empty state
        state.removeAllChildren(nullptr);

        CHECK(sl.getObjects().size() == 0);
        CHECK(pl.getObjects().size() == 0);
        CHECK(fl.getObjects().size() == 0);
        CHECK(el.getObjects().size() == 0);
        CHECK(state.getNumChildren() == 0);

        // register added listeners: NB: not before this point
        std::map<juce::String, bool> callbacks {{"sl", false},
                                                {"pl", false},
                                                {"fl", false},
                                                {"el", false}};

        sl.onObjectAdded = [&callbacks](Source) {
            callbacks["sl"] = true;
        };
        pl.onObjectAdded = [&callbacks](Particle) {
            callbacks["pl"] = true;
        };
        fl.onObjectAdded = [&callbacks](Figure) {
            callbacks["fl"] = true;
        };
        el.onObjectAdded = [&callbacks](Event) {
            callbacks["el"] = true;
        };

        // load state from file
        projectState.load(file);

        CHECK(sl.getObjects().size() == 1);
        CHECK(pl.getObjects().size() == 1);
        CHECK(fl.getObjects().size() == 1);
        CHECK(el.getObjects().size() == 1);
        CHECK(state.getNumChildren() == 4);

        // check listeners called
        for(const auto &callback : callbacks) {
            CHECK(callback.second);
        }
    }

    SECTION("onObjectRemoved")
    {
        // save state to file (empty)
        projectState.save(file);

        // fill state
        auto sl = projectState.getSourceList();
        Source s(sourceFile);
        sl.addObject(s);

        auto pl = projectState.getParticleList();
        Particle p(s);
        pl.addObject(p);

        auto fl = projectState.getFigureList();
        Figure f;
        fl.addObject(f);

        auto el = projectState.getEventList();
        Event e(f, p, 0);
        el.addObject(e);

        CHECK(sl.getObjects().size() == 1);
        CHECK(pl.getObjects().size() == 1);
        CHECK(fl.getObjects().size() == 1);
        CHECK(el.getObjects().size() == 1);
        CHECK(state.getNumChildren() == 4);

        // register remove listeners: NB: not before this point
        std::map<juce::String, bool> callbacks {{"sl", false},
                                                {"pl", false},
                                                {"fl", false},
                                                {"el", false}};

        sl.onObjectRemoved = [&callbacks](Source) {
            callbacks["sl"] = true;
        };
        pl.onObjectRemoved = [&callbacks](Particle) {
            callbacks["pl"] = true;
        };
        fl.onObjectRemoved = [&callbacks](Figure) {
            callbacks["fl"] = true;
        };
        el.onObjectRemoved = [&callbacks](Event) {
            callbacks["el"] = true;
        };

        // load file
        projectState.load(file);

        CHECK(sl.getObjects().size() == 0);
        CHECK(pl.getObjects().size() == 0);
        CHECK(fl.getObjects().size() == 0);
        CHECK(el.getObjects().size() == 0);
        CHECK(state.getNumChildren() == 0);

        // check listeners called
        for(const auto &callback : callbacks) {
            CHECK(callback.second);
        }
    }

    file.deleteFile(); // clean up
}

SCENARIO("Project state: changes to state updates status")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    filepath += FileHelpers::getTestFileName() + ".xml";
    juce::File file(filepath);
    file.create();

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);
    bool cbCalled = false;
    std::unique_ptr<ProjectState::Status> cbStatus;
    std::unique_ptr<ProjectState::Action> cbAction;
    projectState.onStatusChanged = [&cbCalled, &cbStatus, &cbAction](auto s,
                                                                     auto a) {
        cbCalled = true;
        cbStatus = std::make_unique<ProjectState::Status>(s);
        cbAction = std::make_unique<ProjectState::Action>(a);
    };

    WHEN("project state is initialised (empty)")
    {
        THEN("status props are false")
        {
            CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(projectState.getStatus().hasFile);
        }
    }

    WHEN("project state has object added")
    {
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());

        THEN("hasUnsavedChanges is true")
        {
            CHECK(projectState.getStatus().hasUnsavedChanges);
        }

        THEN("callback is called")
        {
            CHECK(cbCalled);
            CHECK(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::StateChange);
        }
    }

    WHEN("project state has object removed")
    {
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());

        CHECK(projectState.getStatus().hasUnsavedChanges);

        // forces hasUnsavedChanges to be false
        projectState.save(file);
        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        cbCalled = false; // reset here as addObject will trigger listener
        cbStatus = nullptr;
        cbAction = nullptr;

        fl.removeObject(fl.getObjects()[0]);
        CHECK(fl.getObjects().size() == 0);

        THEN("hasUnsavedChanges is true")
        {
            CHECK(projectState.getStatus().hasUnsavedChanges);
        }

        THEN("callback is called")
        {
            CHECK(cbCalled);
            CHECK(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::StateChange);
        }
    }

    WHEN("an object in project state is updated")
    {
        auto pathBase = juce::String(ASSETS_DIR) + "/audio";
        auto sourcePath = pathBase + "/whitenoise-2000ms.wav";
        juce::File sourceFile(sourcePath);

        auto sl = projectState.getSourceList();
        Source s(sourceFile);
        sl.addObject(s);

        auto pl = projectState.getParticleList();
        Particle p(s);
        pl.addObject(p);

        CHECK(projectState.getStatus().hasUnsavedChanges);

        // forces hasUnsavedChanges to be false
        projectState.save(file);

        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        cbCalled = false; // reset here as addObject will trigger listener
        cbStatus = nullptr;
        cbAction = nullptr;

        // update particle
        p.setStart(0.001);

        THEN("hasUnsavedChanges is true")
        {
            CHECK(projectState.getStatus().hasUnsavedChanges);
        }

        THEN("callback is called")
        {
            CHECK(cbCalled);
            CHECK(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::StateChange);
        }
    }

    file.deleteFile(); // clean up
}
