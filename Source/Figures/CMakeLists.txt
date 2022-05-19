target_sources(Montage PRIVATE
    DurationProtocolConfig.cpp
    DurationProtocolConfig.h
    DurationProtocolParams.cpp
    DurationProtocolParams.h
    DurationProtocolSelector.cpp
    DurationProtocolSelector.h
    FigureGenerator.cpp
    FigureGenerator.h
    FigureProcessor.cpp
    FigureProcessor.h
    NumberProtocolConfig.cpp
    NumberProtocolConfig.h
    NumberProtocolSelector.cpp
    NumberProtocolSelector.h
    NumberProtocolSettings.cpp
    NumberProtocolSettings.h
    SettingsReview.h
    SettingsReview.cpp
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Tests PRIVATE
    DurationProtocolParams.cpp
    DurationProtocolParams.h
    FigureProcessor.cpp
    FigureProcessor.h
    NumberProtocolSettings.cpp
    NumberProtocolSettings.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

add_subdirectory(NumberProtocolControllers)
add_subdirectory(DurationProtocolControllers)
