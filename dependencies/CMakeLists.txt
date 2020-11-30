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
    GIT_TAG         901b3a2ec0276bc4fbd327381aaa8fda2222824c #v0.1.0-beta
)

FetchContent_MakeAvailable(JUCE tracktion Aleatoric)

juce_add_modules(
    ${tracktion_SOURCE_DIR}/modules/tracktion_engine
    ${tracktion_SOURCE_DIR}/modules/tracktion_graph
)
