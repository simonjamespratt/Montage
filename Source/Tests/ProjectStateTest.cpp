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
SCENARIO("Project state: create")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    ProjectState projectState;

    bool cbCalled = false;
    std::unique_ptr<ProjectState::Status> cbStatus;
    std::unique_ptr<ProjectState::Action> cbAction;
    projectState.onStatusChanged = [&cbCalled, &cbStatus, &cbAction](auto s,
                                                                     auto a) {
        cbCalled = true;
        cbStatus = std::make_unique<ProjectState::Status>(s);
        cbAction = std::make_unique<ProjectState::Action>(a);
    };

    // Initial values
    CHECK(projectState.getProjectDirectory() == nullptr);
    CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
    CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);

    WHEN("The directory param does not exist")
    {
        juce::File directory;
        CHECK_THROWS_AS(projectState.create(directory), InvalidDirectoryPath);

        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        CHECK_FALSE(cbCalled);
        CHECK(cbStatus == nullptr);
        CHECK(cbAction == nullptr);
    }

    WHEN("The directory param is a file (not a directory")
    {
        filepath += FileHelpers::getTestFileName() + ".txt";
        juce::File file(filepath);
        file.create();
        CHECK_THROWS_AS(projectState.create(file), InvalidDirectoryPath);

        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        CHECK_FALSE(cbCalled);
        CHECK(cbStatus == nullptr);
        CHECK(cbAction == nullptr);

        file.deleteFile(); // clean up
    }

    WHEN("The directory exists but has existing files in it")
    {
        auto projectName = FileHelpers::getTestFileName();
        filepath += projectName;
        juce::File directory(filepath);
        directory.createDirectory();
        directory.getChildFile("foo.txt").create();
        CHECK_THROWS_AS(projectState.create(directory),
                        InvalidDirectoryForProjectCreation);

        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        CHECK_FALSE(cbCalled);
        CHECK(cbStatus == nullptr);
        CHECK(cbAction == nullptr);

        directory.deleteRecursively(); // clean up
    }

    WHEN("The directory exists but has existing sub directories in it")
    {
        auto projectName = FileHelpers::getTestFileName();
        filepath += projectName;
        juce::File directory(filepath);
        directory.createDirectory();
        directory.getChildFile("foo").createDirectory();
        CHECK_THROWS_AS(projectState.create(directory),
                        InvalidDirectoryForProjectCreation);

        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        CHECK_FALSE(cbCalled);
        CHECK(cbStatus == nullptr);
        CHECK(cbAction == nullptr);

        directory.deleteRecursively(); // clean up
    }

    WHEN("The directory param points to a valid directory")
    {
        auto projectName = FileHelpers::getTestFileName();
        filepath += projectName;
        juce::File directory(filepath);
        directory.createDirectory();

        // add some existing state
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());
        CHECK(fl.getObjects().size() == 1);

        // reset callbacks
        cbCalled = false;
        cbStatus = nullptr;
        cbAction = nullptr;

        projectState.create(directory);

        THEN("The directory is stored in project state")
        {
            CHECK(*projectState.getProjectDirectory() == directory);
        }

        THEN("The required files and directories are created for a project")
        {
            auto projectDirectory = projectState.getProjectDirectory();
            CHECK(projectDirectory->getChildFile("project-state.xml")
                      .existsAsFile());
            CHECK(projectDirectory->getChildFile("figures").isDirectory());
            CHECK(projectDirectory->getChildFile("renders").isDirectory());
        }

        THEN("The main project state file has a valid xml representation of an "
             "empty project, discarding pre-existing state in the process")
        {
            auto projectStateFile =
                projectState.getProjectDirectory()->getChildFile(
                    "project-state.xml");
            CHECK(projectStateFile.existsAsFile());

            auto tempState =
                juce::ValueTree::fromXml(*juce::parseXML(projectStateFile));

            CHECK(tempState.hasType(IDs::PROJECT_STATE));
            CHECK(tempState.getNumProperties() == 0);
            CHECK(tempState.getNumChildren() == 0);
        }

        THEN("Status is updated and callback is called with correct action")
        {
            CHECK(projectState.getStatus().hasProjectDirectory);
            CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            CHECK(cbCalled);
            CHECK(cbStatus->hasProjectDirectory);
            CHECK_FALSE(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::CreateProject);
        }

        directory.deleteRecursively(); // clean up
    }
}

SCENARIO("Project state: save")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    WHEN("project state does not have a project directory")
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
        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK(projectState.getStatus().hasUnsavedChanges);

        CHECK_THROWS_AS(projectState.save(), ProjectDirectoryNotFound);

        CHECK(projectState.getProjectDirectory() == nullptr);
        CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
        CHECK(projectState.getStatus().hasUnsavedChanges);
        CHECK_FALSE(cbCalled);
        CHECK(cbStatus == nullptr);
        CHECK(cbAction == nullptr);
    }

    WHEN("project state does have a project directory")
    {
        auto projectName = FileHelpers::getTestFileName();
        filepath += projectName;
        juce::File directory(filepath);
        directory.createDirectory();

        projectState.create(directory);

        CHECK_FALSE(projectState.getProjectDirectory() == nullptr);
        CHECK(projectState.getStatus().hasProjectDirectory);

        // fill state
        auto fl = projectState.getFigureList();
        fl.addObject(Figure());

        CHECK(projectState.getStatus().hasUnsavedChanges);

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

        projectState.save();

        THEN("state is saved to the file in the project directory")
        {
            auto projectStateFile =
                projectState.getProjectDirectory()->getChildFile(
                    "project-state.xml");
            CHECK(projectStateFile.existsAsFile());

            auto tempState =
                juce::ValueTree::fromXml(*juce::parseXML(projectStateFile));

            CHECK(tempState.hasType(IDs::PROJECT_STATE));
            CHECK(tempState.getNumProperties() == 0);
            CHECK(tempState.getNumChildren() == 1);
            CHECK(tempState.getChild(0).hasType(IDs::FIGURE));
        }

        THEN("status hasUnsavedChanges is false")
        {
            CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
        }

        THEN("callback is called")
        {
            CHECK(cbCalled);
            CHECK_FALSE(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::SaveProject);
        }

        directory.deleteRecursively(); // clean up
    }
}

SCENARIO("Project state: load")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    // fill state
    auto fl = projectState.getFigureList();
    fl.addObject(Figure());

    CHECK(state.getNumChildren() == 1);
    CHECK(projectState.getProjectDirectory() == nullptr);
    CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
    CHECK(projectState.getStatus().hasUnsavedChanges);

    // NB: register listener after set up
    bool cbCalled = false;
    std::unique_ptr<ProjectState::Status> cbStatus;
    std::unique_ptr<ProjectState::Action> cbAction;
    projectState.onStatusChanged = [&cbCalled, &cbStatus, &cbAction](auto s,
                                                                     auto a) {
        cbCalled = true;
        cbStatus = std::make_unique<ProjectState::Status>(s);
        cbAction = std::make_unique<ProjectState::Action>(a);
    };

    WHEN("An invalid project directory is provided")
    {
        SECTION("The path does not exist")
        {
            juce::File directory;
            CHECK_THROWS_AS(projectState.load(directory), InvalidDirectoryPath);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);
        }

        SECTION("It is a file, not a directory")
        {
            filepath += FileHelpers::getTestFileName() + ".txt";
            juce::File file(filepath);
            file.create();

            CHECK_THROWS_AS(projectState.load(file), InvalidDirectoryPath);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            file.deleteFile(); // clean up
        }

        SECTION("Directory does not have a main project state file")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            directory.getChildFile("figures").createDirectory();
            directory.getChildFile("renders").createDirectory();

            CHECK_THROWS_AS(projectState.load(directory),
                            InvalidDirectoryForProjectLoad);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Directory does not have a figures sub directory")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            directory.getChildFile("project-state.xml").create();
            directory.getChildFile("renders").createDirectory();

            CHECK_THROWS_AS(projectState.load(directory),
                            InvalidDirectoryForProjectLoad);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Directory does not have a renders sub directory")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            directory.getChildFile("project-state.xml").create();
            directory.getChildFile("figures").createDirectory();

            CHECK_THROWS_AS(projectState.load(directory),
                            InvalidDirectoryForProjectLoad);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Project state file is empty")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            directory.getChildFile("project-state.xml").create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            CHECK_THROWS_AS(projectState.load(directory), InvalidProjectFile);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Project state file contains invalid xml")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            auto file = directory.getChildFile("project-state.xml");
            file.create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            juce::FileOutputStream os(file);
            os.writeText("<foo>", false, false, nullptr); // no end tag
            os.flush();

            auto fileContent = file.createInputStream()->readString();
            CHECK(fileContent == "<foo>");

            CHECK_THROWS_AS(projectState.load(directory), InvalidProjectFile);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION(
            "Project state file has a top level value tree of the wrong type")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            auto file = directory.getChildFile("project-state.xml");
            file.create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            juce::FileOutputStream os(file);
            os.writeText(
                "<?xml version=\" 1.0 \" encoding=\"UTF - 8 \"?><foo></foo>",
                false,
                false,
                nullptr);
            os.flush();

            CHECK_THROWS_AS(projectState.load(directory), InvalidProjectFile);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Project state file has top level value tree with props")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            auto file = directory.getChildFile("project-state.xml");
            file.create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            juce::FileOutputStream os(file);
            os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                         "\"?><PROJECT_STATE foo=\"bar\" />",
                         false,
                         false,
                         nullptr);
            os.flush();

            CHECK_THROWS_AS(projectState.load(directory), InvalidProjectFile);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Project state file has invalid children (wrong type)")
        {
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            auto file = directory.getChildFile("project-state.xml");
            file.create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            juce::FileOutputStream os(file);
            os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                         "\"?><PROJECT_STATE><FOO></FOO></PROJECT_STATE>",
                         false,
                         false,
                         nullptr);
            os.flush();

            CHECK_THROWS_AS(projectState.load(directory), InvalidProjectFile);
            CHECK(state.getNumChildren() == 1);
            CHECK(projectState.getProjectDirectory() == nullptr);
            CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
            CHECK(projectState.getStatus().hasUnsavedChanges);
            CHECK_FALSE(cbCalled);
            CHECK(cbStatus == nullptr);
            CHECK(cbAction == nullptr);

            directory.deleteRecursively(); // clean up
        }

        SECTION("Project state file has invalid representations of objects")
        {
            // Easiest way to check this is to include children with valid types
            // but no props. That ensures that something will throw an error
            // somewhere. There is no point checking all reasons that there
            // might be invalid representations of data as this is just a repeat
            // of the object and list tests. Just need to know that before
            // replacing the state with the one loaded from file, that we have
            // tried to create object lists for each object type.
            auto projectName = FileHelpers::getTestFileName();
            filepath += projectName;
            juce::File directory(filepath);
            directory.createDirectory();
            auto file = directory.getChildFile("project-state.xml");
            file.create();
            directory.getChildFile("renders").createDirectory();
            directory.getChildFile("figures").createDirectory();

            juce::FileOutputStream os(file);
            juce::String textContent =
                "<?xml version=\" 1.0 \" encoding=\" UTF - 8 \"?>";

            SECTION("Event")
            {
                textContent += "<PROJECT_STATE><EVENT/></PROJECT_STATE>";
                os.writeText(textContent, false, false, nullptr);
                os.flush();

                CHECK_THROWS_AS(projectState.load(directory),
                                InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getProjectDirectory() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
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

                CHECK_THROWS_AS(projectState.load(directory),
                                InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getProjectDirectory() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
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

                CHECK_THROWS_AS(projectState.load(directory),
                                InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getProjectDirectory() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
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

                CHECK_THROWS_AS(projectState.load(directory),
                                InvalidProjectFile);
                CHECK(state.getNumChildren() == 1);
                CHECK(projectState.getProjectDirectory() == nullptr);
                CHECK_FALSE(projectState.getStatus().hasProjectDirectory);
                CHECK(projectState.getStatus().hasUnsavedChanges);
                CHECK_FALSE(cbCalled);
                CHECK(cbStatus == nullptr);
                CHECK(cbAction == nullptr);
            }

            directory.deleteRecursively(); // clean up
        }
    }

    WHEN("An valid project directory is provided")
    {
        auto projectName = FileHelpers::getTestFileName();
        filepath += projectName;
        juce::File directory(filepath);
        directory.createDirectory();
        auto file = directory.getChildFile("project-state.xml");
        file.create();
        directory.getChildFile("renders").createDirectory();
        directory.getChildFile("figures").createDirectory();

        juce::FileOutputStream os(file);
        os.writeText("<?xml version=\" 1.0 \" encoding=\" UTF - 8 "
                     "\"?><PROJECT_STATE></PROJECT_STATE>",
                     false,
                     false,
                     nullptr);
        os.flush();

        projectState.load(directory);

        THEN("state should be changed")
        {
            CHECK(state.getNumChildren() == 0);
        }

        THEN("the provided directory is stored in project state")
        {
            CHECK(*projectState.getProjectDirectory() == directory);
        }

        THEN("status is updated and callback is called with correct action")
        {
            CHECK(projectState.getStatus().hasProjectDirectory);
            CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            CHECK(cbCalled);
            CHECK(cbStatus->hasProjectDirectory);
            CHECK_FALSE(cbStatus->hasUnsavedChanges);
            CHECK(*cbAction == ProjectState::Action::LoadProject);
        }

        directory.deleteRecursively(); // clean up
    }
}

SCENARIO("Project state: loading a project notifies list listeners")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    auto projectName = FileHelpers::getTestFileName();
    filepath += projectName;
    juce::File directory(filepath);
    directory.createDirectory();

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    projectState.create(directory);

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
        projectState.save();

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
        projectState.load(directory);

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
        projectState.save();

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
        projectState.load(directory);

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

    directory.deleteRecursively(); // clean up
}

SCENARIO("Project state: changes to state updates status")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    auto projectName = FileHelpers::getTestFileName();
    filepath += projectName;
    juce::File directory(filepath);
    directory.createDirectory();

    // state empty
    juce::ValueTree state(IDs::PROJECT_STATE);
    ProjectState projectState(state);

    projectState.create(directory);

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
        THEN("check status props")
        {
            CHECK_FALSE(projectState.getStatus().hasUnsavedChanges);
            CHECK(projectState.getStatus().hasProjectDirectory);
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
        projectState.save();
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
        projectState.save();

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

    directory.deleteRecursively(); // clean up
}
