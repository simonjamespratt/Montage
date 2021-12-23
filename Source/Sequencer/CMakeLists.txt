add_subdirectory(Plugins)

target_sources(Montage PRIVATE
    Arrangement.cpp
    Arrangement.h
    LevelMeterView.cpp
    LevelMeterView.h
    Sequencer.cpp
    Sequencer.h
    SequencerControlsView.cpp
    SequencerControlsView.h
    SequencerViewState.h
    Timeline.cpp
    Timeline.h
    TrackControlPanel.cpp
    TrackControlPanel.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
