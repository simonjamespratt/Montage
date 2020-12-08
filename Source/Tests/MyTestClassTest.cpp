#include "MyTestClass.h"

#include "TestIdentifiers.h"

#include <Range.hpp>
#include <catch2/catch.hpp>
#include <juce_data_structures/juce_data_structures.h>

SCENARIO("Test MyTestClass")
{
    MyTestClass instance;
    juce::ValueTree myNode(myNodeType);
    myNode.setProperty(rangeStart, 10, nullptr);
    myNode.setProperty(rangeEnd, 20, nullptr);
    auto range = instance.someMethod(myNode);
    REQUIRE(range.start == 10);
    REQUIRE(range.end == 20);
}
