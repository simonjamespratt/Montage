include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE
    GIT_TAG         b8206e3604ebaca64779bf19f1613c373b9adf4f #v6.0.4
)

FetchContent_Declare(
    tracktion
    GIT_REPOSITORY  https://github.com/Tracktion/tracktion_engine
    GIT_TAG         e1cfb5aa6423891803a42126eeec4d077a6c12aa # develop branch on 30-11-2020
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

juce_add_modules(
    ${tracktion_SOURCE_DIR}/modules/tracktion_engine
    ${tracktion_SOURCE_DIR}/modules/tracktion_graph
)
