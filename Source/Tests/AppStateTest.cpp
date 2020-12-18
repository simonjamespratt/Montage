#include <catch2/catch.hpp>

TEST_CASE("Creating a file")
{
    REQUIRE(CURRENT_BINARY_DIRECTORY == "");
}
