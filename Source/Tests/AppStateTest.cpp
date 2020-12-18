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
