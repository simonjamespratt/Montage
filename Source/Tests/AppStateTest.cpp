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

    // change state before reloading
    state.removeProperty(IDs::id, nullptr);
    state.setProperty(IDs::onset, 456, nullptr);

    CHECK(appState.loadStateFromFile(file));

    CHECK_FALSE(state.hasProperty(IDs::id));
    CHECK(state.hasProperty(IDs::onset));

    // NB: don't remove this line unless also remove file.create() above as this
    // is clean up
    file.deleteFile();
}
