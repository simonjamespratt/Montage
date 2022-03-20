target_sources(Tests PRIVATE
    main.cpp
    DurationProtocolParamsTest.cpp
    EventTest.cpp
    EventListTest.cpp
    FigureListTest.cpp
    FigureProcessorTest.cpp
    FigureTest.cpp
    NumberProtocolSettingsTest.cpp
    ParticleListTest.cpp
    ParticleTest.cpp
    ProjectStateTest.cpp
    SourceTest.cpp
    SourceListTest.cpp
    VariantConvertersTest.cpp
)

add_subdirectory(Helpers)

target_compile_definitions(Tests
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call
        ASSETS_DIR="${montage_SOURCE_DIR}/assets"
        CURRENT_BINARY_DIRECTORY="${CMAKE_CURRENT_BINARY_DIR}" # Used for creating temp files for testing purposes
)

target_link_libraries(Tests
    PRIVATE
        juce::juce_core
        juce::juce_data_structures
        juce::juce_audio_formats
        Catch2::Catch2
        Aleatoric::Aleatoric
)
