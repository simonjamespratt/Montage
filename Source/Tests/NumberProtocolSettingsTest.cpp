#include "NumberProtocolSettings.h"

#include <catch2/catch.hpp>

SCENARIO("NumberProtocolSettings: receives aleatoric params and sets state")
{
    SECTION("Adjacent Steps protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::AdjacentStepsParams {});

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::adjacentSteps);
    }

    SECTION("Basic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::basic);
    }

    SECTION("Cycle protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::CycleParams(true, true));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::cycle);
        REQUIRE(settings.cycle.bidirectional == true);
        REQUIRE(settings.cycle.reverseDirection == true);
    }

    SECTION("Granular Walk protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GranularWalkParams(0.5));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::granularWalk);
        REQUIRE(settings.granularWalk.deviationFactor == 0.5);
    }

    SECTION("Grouped Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GroupedRepetitionParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::groupedRepetition);
        REQUIRE(settings.groupedRepetition.groupings ==
                std::vector<int> {1, 2, 3});
    }

    SECTION("No Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::NoRepetitionParams {});

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::noRepetition);
    }

    SECTION("Periodic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::PeriodicParams(0.5));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::periodic);
        REQUIRE(settings.periodic.chanceOfRepetition == 0.5);
    }

    SECTION("Precision protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::PrecisionParams(std::vector<double> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::precision);
        REQUIRE(settings.precision.distribution ==
                std::vector<double> {1, 2, 3});
    }

    SECTION("Ratio protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::RatioParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::ratio);
        REQUIRE(settings.ratio.ratios == std::vector<int> {1, 2, 3});
    }

    SECTION("Serial Params")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SerialParams {});

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::serial);
    }

    SECTION("Subset protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SubsetParams(10, 20));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::subset);
        REQUIRE(settings.subset.min == 10);
        REQUIRE(settings.subset.max == 20);
    }

    SECTION("Walk protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::WalkParams(10));

        NumberProtocolSettings settings(params);
        REQUIRE(settings.activeType == NumberProtocolType::walk);
        REQUIRE(settings.walk.maxStep == 10);
    }

    SECTION("Context: none (default)")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params);
        REQUIRE(settings.context == CreationContext::none);
    }

    SECTION("Context: duration_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::duration_selection);
        REQUIRE(settings.context == CreationContext::duration_selection);
    }

    SECTION("Context: particle_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::particle_selection);
        REQUIRE(settings.context == CreationContext::particle_selection);
    }
}

SCENARIO("NumberProtocolSettings: copy constrcutor")
{
    SECTION("Adjacent Steps protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::AdjacentStepsParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::adjacentSteps);
        REQUIRE(copied.activeType == NumberProtocolType::adjacentSteps);
    }

    SECTION("Basic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::basic);
        REQUIRE(copied.activeType == NumberProtocolType::basic);
    }

    SECTION("Cycle protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::CycleParams(true, true));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::cycle);
        REQUIRE(settings.cycle.bidirectional == true);
        REQUIRE(settings.cycle.reverseDirection == true);

        REQUIRE(copied.activeType == NumberProtocolType::cycle);
        REQUIRE(copied.cycle.bidirectional == true);
        REQUIRE(copied.cycle.reverseDirection == true);
    }

    SECTION("Granular Walk protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GranularWalkParams(0.5));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::granularWalk);
        REQUIRE(settings.granularWalk.deviationFactor == 0.5);

        REQUIRE(copied.activeType == NumberProtocolType::granularWalk);
        REQUIRE(copied.granularWalk.deviationFactor == 0.5);
    }

    SECTION("Grouped Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GroupedRepetitionParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::groupedRepetition);
        REQUIRE(settings.groupedRepetition.groupings ==
                std::vector<int> {1, 2, 3});

        REQUIRE(copied.activeType == NumberProtocolType::groupedRepetition);
        REQUIRE(copied.groupedRepetition.groupings ==
                std::vector<int> {1, 2, 3});
    }

    SECTION("No Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::NoRepetitionParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::noRepetition);
        REQUIRE(copied.activeType == NumberProtocolType::noRepetition);
    }

    SECTION("Periodic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::PeriodicParams(0.5));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::periodic);
        REQUIRE(settings.periodic.chanceOfRepetition == 0.5);

        REQUIRE(copied.activeType == NumberProtocolType::periodic);
        REQUIRE(copied.periodic.chanceOfRepetition == 0.5);
    }

    SECTION("Precision protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::PrecisionParams(std::vector<double> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::precision);
        REQUIRE(settings.precision.distribution ==
                std::vector<double> {1, 2, 3});

        REQUIRE(copied.activeType == NumberProtocolType::precision);
        REQUIRE(copied.precision.distribution == std::vector<double> {1, 2, 3});
    }

    SECTION("Ratio protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::RatioParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::ratio);
        REQUIRE(settings.ratio.ratios == std::vector<int> {1, 2, 3});

        REQUIRE(copied.activeType == NumberProtocolType::ratio);
        REQUIRE(copied.ratio.ratios == std::vector<int> {1, 2, 3});
    }

    SECTION("Serial Params")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SerialParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::serial);
        REQUIRE(copied.activeType == NumberProtocolType::serial);
    }

    SECTION("Subset protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SubsetParams(10, 20));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::subset);
        REQUIRE(settings.subset.min == 10);
        REQUIRE(settings.subset.max == 20);

        REQUIRE(copied.activeType == NumberProtocolType::subset);
        REQUIRE(copied.subset.min == 10);
        REQUIRE(copied.subset.max == 20);
    }

    SECTION("Walk protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::WalkParams(10));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(settings.activeType == NumberProtocolType::walk);
        REQUIRE(settings.walk.maxStep == 10);

        REQUIRE(copied.activeType == NumberProtocolType::walk);
        REQUIRE(copied.walk.maxStep == 10);
    }

    SECTION("Context: none (default)")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params);
        NumberProtocolSettings copied(settings);

        REQUIRE(copied.context == CreationContext::none);
    }

    SECTION("Context: duration_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::duration_selection);
        NumberProtocolSettings copied(settings);

        REQUIRE(copied.context == CreationContext::duration_selection);
    }

    SECTION("Context: particle_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::particle_selection);
        NumberProtocolSettings copied(settings);

        REQUIRE(copied.context == CreationContext::particle_selection);
    }
}

SCENARIO("NumberProtocolSettings: from value tree state")
{
    SECTION("Adjacent Steps protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::AdjacentStepsParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::adjacentSteps);
    }

    SECTION("Basic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::basic);
    }

    SECTION("Cycle protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::CycleParams(true, true));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::cycle);
        REQUIRE(fromState.cycle.bidirectional == true);
        REQUIRE(fromState.cycle.reverseDirection == true);
    }

    SECTION("Granular Walk protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GranularWalkParams(0.5));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::granularWalk);
        REQUIRE(fromState.granularWalk.deviationFactor == 0.5);
    }

    SECTION("Grouped Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::GroupedRepetitionParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::groupedRepetition);
        REQUIRE(fromState.groupedRepetition.groupings ==
                std::vector<int> {1, 2, 3});
    }

    SECTION("No Repetition protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::NoRepetitionParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::noRepetition);
    }

    SECTION("Periodic protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::PeriodicParams(0.5));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::periodic);
        REQUIRE(fromState.periodic.chanceOfRepetition == 0.5);
    }

    SECTION("Precision protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::PrecisionParams(std::vector<double> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::precision);
        REQUIRE(fromState.precision.distribution ==
                std::vector<double> {1, 2, 3});
    }

    SECTION("Ratio protocol")
    {
        aleatoric::NumberProtocolParams params(
            aleatoric::RatioParams(std::vector<int> {1, 2, 3}));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::ratio);
        REQUIRE(fromState.ratio.ratios == std::vector<int> {1, 2, 3});
    }

    SECTION("Serial Params")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SerialParams {});

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::serial);
    }

    SECTION("Subset protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::SubsetParams(10, 20));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::subset);
        REQUIRE(fromState.subset.min == 10);
        REQUIRE(fromState.subset.max == 20);
    }

    SECTION("Walk protocol")
    {
        aleatoric::NumberProtocolParams params(aleatoric::WalkParams(10));

        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.activeType == NumberProtocolType::walk);
        REQUIRE(fromState.walk.maxStep == 10);
    }

    SECTION("Context: none (default)")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.context == CreationContext::none);
    }

    SECTION("Context: duration_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::duration_selection);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.context == CreationContext::duration_selection);
    }

    SECTION("Context: particle_selection")
    {
        aleatoric::NumberProtocolParams params(aleatoric::BasicParams {});
        NumberProtocolSettings settings(params,
                                        CreationContext::particle_selection);
        NumberProtocolSettings fromState(settings.state);

        REQUIRE(fromState.context == CreationContext::particle_selection);
    }
}
