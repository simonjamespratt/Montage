#include "VariantConverters.h"

#include <catch2/catch.hpp>

SCENARIO("Variant Converter: juce::Array<int>")
{
    juce::Identifier vtId("vt");
    juce::Identifier propId("prop");

    juce::ValueTree vt(vtId);
    vt.setProperty(propId,
                   juce::var(juce::Array<juce::var>(123, 456)),
                   nullptr);

    juce::CachedValue<juce::Array<int>> value;
    value.referTo(vt, propId, nullptr);

    REQUIRE(value.get() == juce::Array<int> {123, 456});

    SECTION("set new value by assignment works")
    {
        value = {135, 246};
        REQUIRE(value.get() == juce::Array<int> {135, 246});
    }

    SECTION("modifying array without assignment does not work")
    {
        value.get().add(246);
        REQUIRE(value.get() == juce::Array<int> {123, 456});
    }

    SECTION("modyfying a copy of the array followed by assignment works")
    {
        auto copy = value.get();
        copy.add(246);
        value = copy;
        REQUIRE(value.get() == juce::Array<int> {123, 456, 246});
    }
}

SCENARIO("Variant Converter: std::vector<int>")
{
    juce::Identifier vtId("vt");
    juce::Identifier propId("prop");

    juce::ValueTree vt(vtId);
    vt.setProperty(propId,
                   juce::var(juce::Array<juce::var>(123, 456)),
                   nullptr);
    juce::CachedValue<std::vector<int>> value;
    value.referTo(vt, propId, nullptr);

    REQUIRE(value.get() == std::vector<int> {123, 456});

    SECTION("set new value by assignment works")
    {
        value = {135, 246};
        REQUIRE(value.get() == std::vector<int> {135, 246});
    }

    SECTION("modifying array without assignment does not work")
    {
        value.get().emplace_back(246);
        REQUIRE(value.get() == std::vector<int> {123, 456});
    }

    SECTION("modyfying a copy of the array followed by assignment works")
    {
        auto copy = value.get();
        copy.emplace_back(246);
        value = copy;
        REQUIRE(value.get() == std::vector<int> {123, 456, 246});
    }
}

SCENARIO("Variant Converter: std::vector<double>")
{
    juce::Identifier vtId("vt");
    juce::Identifier propId("prop");

    juce::ValueTree vt(vtId);
    vt.setProperty(propId,
                   juce::var(juce::Array<juce::var>(123, 456)),
                   nullptr);
    juce::CachedValue<std::vector<double>> value;
    value.referTo(vt, propId, nullptr);

    REQUIRE(value.get() == std::vector<double> {123, 456});

    SECTION("set new value by assignment works")
    {
        value = {135, 246};
        REQUIRE(value.get() == std::vector<double> {135, 246});
    }

    SECTION("modifying array without assignment does not work")
    {
        value.get().emplace_back(246);
        REQUIRE(value.get() == std::vector<double> {123, 456});
    }

    SECTION("modyfying a copy of the array followed by assignment works")
    {
        auto copy = value.get();
        copy.emplace_back(246);
        value = copy;
        REQUIRE(value.get() == std::vector<double> {123, 456, 246});
    }
}
