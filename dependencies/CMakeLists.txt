include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE
    GIT_TAG         b4bc2c8710cd1b0ca0e992040c16f89c7e137e70 # develop branch on 23-11-2021
)

FetchContent_Declare(
    tracktion
    GIT_REPOSITORY  https://github.com/Tracktion/tracktion_engine
    GIT_TAG         50bd60a93ff54a297b3ed2f2e6f9f238cddf8605 # develop branch on 24-11-2021
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
