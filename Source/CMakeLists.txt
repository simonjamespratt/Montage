juce_add_gui_app(Montage)

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

target_compile_definitions(Montage PRIVATE
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Montage,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Montage,JUCE_VERSION>"
)

target_link_libraries(Montage
    PRIVATE
        juce::juce_gui_basics
        juce::juce_core
        juce::juce_data_structures
        tracktion_engine
        tracktion_graph
        Aleatoric::Aleatoric
    # TODO: add juce recommended flags
    # PUBLIC
    #     juce::juce_recommended_config_flags
    #     juce::juce_recommended_lto_flags
    #     juce::juce_recommended_warning_flags
)
