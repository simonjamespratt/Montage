target_sources(Tests PRIVATE
    main.cpp
    AppStateTest.cpp
    EventTest.cpp
    FigureTest.cpp
    FigureCollectionTest.cpp
    FigureProcessorTest.cpp
    ParticleCollectionTest.cpp
    ParticleTest.cpp
)

add_subdirectory(Helpers)

target_compile_definitions(Tests
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call
        CURRENT_BINARY_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}" # Used for creating temp files for testing purposes
)

target_link_libraries(Tests
    PRIVATE
        juce::juce_core
        juce::juce_data_structures
        Catch2::Catch2
        Aleatoric::Aleatoric
)
