target_sources(Montage PRIVATE
    FigureManager.cpp
    FigureManager.h
    FileManager.cpp
    FileManager.h
    ParticlesManifest.cpp
    ParticlesManifest.h
    SourceManager.cpp
    SourceManager.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
