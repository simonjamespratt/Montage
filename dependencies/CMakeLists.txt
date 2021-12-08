include(FetchContent)

# NB: keep juce and tracktion in sync where possible
# (see: https://forum.juce.com/t/two-errors-wile-compiling-latest-tracktion-with-juce-6-0-8/46339/4).
# Or alternatively point both to the top of their develop branches which should be pretty in sync.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE
    GIT_TAG         461192b35506ef0c5feb43235c916d7b7db2d802
)

FetchContent_Declare(
    tracktion
    GIT_REPOSITORY  https://github.com/Tracktion/tracktion_engine
    GIT_TAG         df5cd92666b4ab3179c617ea72e9655f3ffa507a
)

FetchContent_Declare(
    Aleatoric
    GIT_REPOSITORY  https://github.com/simonjamespratt/Aleatoric.git
    GIT_TAG         master
)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
        GIT_TAG         ff349a50bfc6214b4081f4ca63c7de35e2162f60 #v2.13.3
)

FetchContent_MakeAvailable(JUCE tracktion Aleatoric Catch2)
