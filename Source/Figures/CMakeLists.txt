target_sources(Montage PRIVATE
    FigureGenerator.cpp
    FigureGenerator.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
