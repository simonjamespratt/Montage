#include "Source.h"

#include "ErrorTypes.h"
#include "StateHelpers.h"

#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>

SCENARIO("Source: receive existing state")
{
    WHEN("Data is invalid")
    {
        SECTION("Incorrect type")
        {
            juce::ValueTree state(IDs::PARTICLE);
            REQUIRE_THROWS_AS(Source(state), std::invalid_argument);
        }

        SECTION("Missing properties")
        {
            juce::ValueTree state(IDs::SOURCE);

            SECTION("No id")
            {
                state.setProperty(IDs::file_path, "filepath", nullptr);
                REQUIRE_THROWS_AS(Source(state), std::invalid_argument);
            }

            SECTION("No file path")
            {
                state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
                REQUIRE_THROWS_AS(Source(state), std::invalid_argument);
            }
        }

        SECTION("Unexpected props present")
        {
            auto state = StateHelpers::createSourceState(juce::Uuid());
            state.setProperty(IDs::PARTICLE, "Unexpected prop", nullptr);
            REQUIRE_THROWS_AS(Source(state), std::invalid_argument);
        }

        SECTION("Invalid file data")
        {
            juce::ValueTree state(IDs::SOURCE);
            state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);

            WHEN("File does not exist")
            {
                state.setProperty(IDs::file_path, "", nullptr);
                REQUIRE_THROWS_AS(Source(state), InvalidFilePath);
            }

            WHEN("File path points to a directory")
            {
                state.setProperty(IDs::file_path,
                                  juce::String(CURRENT_BINARY_DIRECTORY),
                                  nullptr);
                REQUIRE_THROWS_AS(Source(state), InvalidFilePath);
            }

            WHEN("File is not of type .wav or .aif .aiff")
            {
                juce::String filepath(CURRENT_BINARY_DIRECTORY);
                filepath += "/testfile.txt";
                juce::File file(filepath);
                file.create();
                state.setProperty(IDs::file_path, filepath, nullptr);
                REQUIRE_THROWS_AS(Source(state), InvalidAudioFile);
                file.deleteFile(); // NB: clean up
            }
        }
    }

    WHEN("Data is valid")
    {
        juce::Uuid id;
        juce::String pathBase(AUDIO_ASSETS_DIR);
        auto filepath = pathBase + "/whitenoise-2000ms.wav";
        double approxLengthInSeconds = 2.0;

        auto state = StateHelpers::createSourceState(id, filepath);

        Source source(state);

        THEN("Getting data is as expected")
        {
            REQUIRE(source.getId() == id);
            REQUIRE(source.getFile() == juce::File(filepath));
            REQUIRE(source.getFileName() == "whitenoise-2000ms");
            REQUIRE(source.getFileLengthInSeconds() ==
                    Approx(2.0).epsilon(0.005));

            auto returnedState = source.getState();
            REQUIRE(returnedState.hasType(IDs::SOURCE));
            REQUIRE(juce::Uuid(returnedState[IDs::id]) == id);
            REQUIRE(returnedState[IDs::file_path] == filepath);
        }
    }
}

SCENARIO("Source: create state from file")
{
    WHEN("Received file data is invalid")
    {
        WHEN("File does not exist")
        {
            juce::File file("foo");
            REQUIRE_THROWS_AS(Source(file), InvalidFilePath);
        }

        WHEN("File path points to a directory")
        {
            juce::File file(juce::String(CURRENT_BINARY_DIRECTORY));
            REQUIRE_THROWS_AS(Source(file), InvalidFilePath);
        }

        WHEN("File is not of type .wav or .aif .aiff")
        {
            juce::String filepath(CURRENT_BINARY_DIRECTORY);
            filepath += "/testfile.txt";
            juce::File file(filepath);
            file.create();
            REQUIRE_THROWS_AS(Source(file), InvalidAudioFile);
            file.deleteFile(); // NB: clean up
        }
    }

    WHEN("Data is valid")
    {
        juce::String pathBase(AUDIO_ASSETS_DIR);
        auto filepath = pathBase + "/whitenoise-2000ms.wav";
        double approxLengthInSeconds = 2.0;
        juce::File file(filepath);
        Source source(file);

        THEN("Getting data is as expected")
        {
            REQUIRE_FALSE(source.getId().isNull());
            REQUIRE(source.getFile() == juce::File(filepath));
            REQUIRE(source.getFileName() == "whitenoise-2000ms");
            REQUIRE(source.getFileLengthInSeconds() ==
                    Approx(2.0).epsilon(0.005));

            auto returnedState = source.getState();
            REQUIRE(returnedState.hasType(IDs::SOURCE));
            REQUIRE(juce::Uuid(returnedState[IDs::id]) == source.getId());
            REQUIRE(returnedState[IDs::file_path] == filepath);
        }
    }
}
