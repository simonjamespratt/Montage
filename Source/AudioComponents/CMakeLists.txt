target_sources(Montage PRIVATE
    AudioEditViewport.cpp
    AudioEditViewport.h
    Cursor.cpp
    Cursor.h
    TimeScalingFactor.h
    TimeDisplay.cpp
    TimeDisplay.h
    TracktionThumbnail.cpp
    TracktionThumbnail.h
    TransportController.cpp
    TransportController.h
    TransportInteractor.cpp
    TransportInteractor.h
    TransportManager.cpp
    TransportManager.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
