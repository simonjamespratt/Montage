#include "AppState.h"

#include "Identifiers.h"

#include <catch2/catch.hpp>
#include <juce_core/juce_core.h>

TEST_CASE("Creating a file")
{
    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    filepath += "/testfile.txt";
    juce::File file(filepath);
    REQUIRE(file.existsAsFile() == false);
    file.create();
    REQUIRE(file.existsAsFile() == true);
    file.deleteFile();
    REQUIRE(file.existsAsFile() == false);
}

TEST_CASE("Save app state")
{
    // TODO: tests

    // try to alter internal state rather than return from loading method

    // check properly what the content of the val tree is - try making it have
    // different props, then loading it

    // when provided file does not exist when

    // the file provided is not .xml

    // review the other checks that tracktion example makes

    juce::ValueTree state(IDs::APP_STATE);
    state.setProperty(IDs::id, 123, nullptr);
    AppState appState(state);

    juce::String filepath(CURRENT_BINARY_DIRECTORY);
    filepath += "/testfile.xml";
    juce::File file(filepath);
    file.create();

    CHECK(appState.saveStateToFile(file));

    // empty state before reloading
    state.removeProperty(IDs::id, nullptr);

    // CHECK(appState.loadStateFromFile(file));

    auto loadedState = appState.loadStateFromFile(file);
    CHECK(loadedState.getNumProperties() == 1);

    // NB: don't remove this line unless also remove file.create() above as this
    // is clean up
    file.deleteFile();
}
