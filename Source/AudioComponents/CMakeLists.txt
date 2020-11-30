target_sources(Montage PRIVATE
    Cursor.cpp
    Cursor.h
    TimeDisplay.cpp
    TimeDisplay.h
    TracktionThumbnail.cpp
    TracktionThumbnail.h
    TransportController.cpp
    TransportController.h
    TransportInteractor.cpp
    TransportInteractor.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
