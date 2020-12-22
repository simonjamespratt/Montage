juce_add_gui_app(Montage)
juce_add_console_app(Tests)

# Populate CMAKE_MODULE_PATH with Catch helper scripts
# This block needs to be done in the file where the Tests target is created, hence here and not in Tests dir
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(Tests)

target_sources(Montage PRIVATE
    Main.cpp
    MainComponent.h
    MainComponent.cpp
)

add_subdirectory(AudioComponents)
add_subdirectory(DataManagement)
add_subdirectory(Errors)
add_subdirectory(Figures)
add_subdirectory(PageLayout)
add_subdirectory(Particles)
add_subdirectory(Sequencer)
add_subdirectory(Utilities)

add_subdirectory(Tests)

target_compile_definitions(Montage PRIVATE
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Montage,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Montage,JUCE_VERSION>"
    AUDIO_ASSETS_DIR="${montage_SOURCE_DIR}/assets/audio"
    PRE_POPULATE_DUMMY_DATA=1 # TODO: CMAKE: this should probably be in a gen expression
)

target_link_libraries(Montage
    PRIVATE
        juce::juce_gui_basics
        juce::juce_core
        juce::juce_data_structures
        tracktion_engine
        tracktion_graph
        Aleatoric::Aleatoric
    # TODO: CMAKE: add juce recommended flags
    # PUBLIC
    #     juce::juce_recommended_config_flags
    #     juce::juce_recommended_lto_flags
    #     juce::juce_recommended_warning_flags
)
