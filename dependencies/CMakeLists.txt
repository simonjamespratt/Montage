include(FetchContent)

# NB: keep juce and tracktion in sync where possible
# (see: https://forum.juce.com/t/two-errors-wile-compiling-latest-tracktion-with-juce-6-0-8/46339/4).
# Or alternatively point both to the top of their develop branches which should be pretty in sync.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE
    GIT_TAG ef8ad92138cd3e813bc5e4035d748bc476108954 # develop 13/04/2022 (verified)
)

FetchContent_Declare(
    tracktion
    GIT_REPOSITORY https://github.com/Tracktion/tracktion_engine
    GIT_TAG e7201ab1e81791c1680275fb70e47eb1af57ba97 # develop 22/04/2022 (verified)
)

FetchContent_Declare(
    Aleatoric
    GIT_REPOSITORY https://github.com/simonjamespratt/Aleatoric.git
    GIT_TAG master
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG ff349a50bfc6214b4081f4ca63c7de35e2162f60 # v2.13.3
)

FetchContent_MakeAvailable(JUCE tracktion Aleatoric Catch2)
