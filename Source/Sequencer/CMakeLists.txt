target_sources(Montage PRIVATE
    Arrangement.cpp
    Arrangement.h
    Sequencer.cpp
    Sequencer.h
    Timeline.cpp
    Timeline.h
    TrackControlPanel.cpp
    TrackControlPanel.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
