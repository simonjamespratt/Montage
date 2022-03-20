#include "DurationProtocolParams.h"

#include "Identifiers.h"

#include <catch2/catch.hpp>

SCENARIO("DurationProtocolSettings: creationSettings")
{
    SECTION("No state received - sets default values")
    {
        DurationProtocolSettings settings;

        REQUIRE(settings.activeType == DurationProtocolType::prescribed);

        REQUIRE(settings.geometric.rangeStart == 1);
        REQUIRE(settings.geometric.rangeEnd == 10);
        REQUIRE(settings.geometric.collectionSize == 5);

        REQUIRE(settings.prescribed.durations.get() ==
                std::vector<int> {1000, 2000});

        REQUIRE(settings.multiples.strategy == MultiplierStrategy::range);
        REQUIRE(settings.multiples.rangeStart == 1);
        REQUIRE(settings.multiples.rangeEnd == 10);
        REQUIRE(settings.multiples.baseIncrement == 100);
        REQUIRE(settings.multiples.deviationFactor == 0);
        REQUIRE(settings.multiples.multipliers.get() ==
                std::vector<int> {1000, 2000});
    }

    SECTION("Receive state - sets values it receives")
    {
        juce::ValueTree state(IDs::DURATION_SETTINGS);
        state.setProperty(IDs::active_type,
                          static_cast<int>(DurationProtocolType::multiples),
                          nullptr);

        juce::ValueTree geoState(IDs::GEOMETRIC_SETTINGS);
        geoState.setProperty(IDs::geo_range_start, 1, nullptr);
        geoState.setProperty(IDs::geo_range_end, 2, nullptr);
        geoState.setProperty(IDs::geo_collection_size, 3, nullptr);
        state.addChild(geoState, -1, nullptr);

        juce::ValueTree prescribedState(IDs::PRESCRIBED_SETTINGS);
        prescribedState.setProperty(IDs::prescribed_durations,
                                    juce::var(juce::Array<juce::var>(4, 5, 6)),
                                    nullptr);
        state.addChild(prescribedState, -1, nullptr);

        juce::ValueTree multiState(IDs::MULTIPLES_SETTINGS);
        multiState.setProperty(IDs::multi_strategy,
                               static_cast<int>(MultiplierStrategy::hand),
                               nullptr);
        multiState.setProperty(IDs::multi_range_start, 1, nullptr);
        multiState.setProperty(IDs::multi_range_end, 2, nullptr);
        multiState.setProperty(IDs::multi_base_increment, 3, nullptr);
        multiState.setProperty(IDs::multi_deviation_factor, 4, nullptr);
        multiState.setProperty(IDs::multi_multipliers,
                               juce::var(juce::Array<juce::var>(5, 6)),
                               nullptr);
        state.addChild(multiState, -1, nullptr);

        DurationProtocolSettings settings(state);
        REQUIRE(settings.activeType == DurationProtocolType::multiples);

        REQUIRE(settings.geometric.rangeStart == 1);
        REQUIRE(settings.geometric.rangeEnd == 2);
        REQUIRE(settings.geometric.collectionSize == 3);

        REQUIRE(settings.prescribed.durations.get() ==
                std::vector<int> {4, 5, 6});

        REQUIRE(settings.multiples.strategy == MultiplierStrategy::hand);
        REQUIRE(settings.multiples.rangeStart == 1);
        REQUIRE(settings.multiples.rangeEnd == 2);
        REQUIRE(settings.multiples.baseIncrement == 3);
        REQUIRE(settings.multiples.deviationFactor == 4);
        REQUIRE(settings.multiples.multipliers == std::vector<int> {5, 6});
    }

    SECTION("Receive DurationProtocolParams - sets state to params values")
    {
        DurationProtocolParams params(DurationProtocolType::multiples);

        params.prescribed.durations = std::vector<int> {1, 2, 3};

        params.geometric.rangeStart = 123;
        params.geometric.rangeEnd = 456;
        params.geometric.collectionSize = 789;

        params.multiples.strategy = MultiplierStrategy::hand;
        params.multiples.rangeStart = 135;
        params.multiples.rangeEnd = 246;
        params.multiples.multipliers = std::vector<int> {2, 4, 6};
        params.multiples.baseIncrement = 1000;
        params.multiples.deviationFactor = 1;

        DurationProtocolSettings settings(params);
        REQUIRE(settings.activeType == DurationProtocolType::multiples);

        REQUIRE(settings.geometric.rangeStart == 123);
        REQUIRE(settings.geometric.rangeEnd == 456);
        REQUIRE(settings.geometric.collectionSize == 789);

        REQUIRE(settings.prescribed.durations.get() ==
                std::vector<int> {1, 2, 3});

        REQUIRE(settings.multiples.strategy == MultiplierStrategy::hand);
        REQUIRE(settings.multiples.rangeStart == 135);
        REQUIRE(settings.multiples.rangeEnd == 246);
        REQUIRE(settings.multiples.multipliers == std::vector<int> {2, 4, 6});
        REQUIRE(settings.multiples.baseIncrement == 1000);
        REQUIRE(settings.multiples.deviationFactor == 1);
    }
}
