target_sources(Montage PRIVATE
    CollectionHelper.cpp
    CollectionHelper.h
    Event.cpp
    Event.h
    Figure.cpp
    Figure.h
    FigureCollection.cpp
    FigureCollection.h
    FigureManager.cpp
    FigureManager.h
    FileManager.cpp
    FileManager.h
    Particle.cpp
    Particle.h
    ParticleCollection.cpp
    ParticleCollection.h
    ParticlesManifest.cpp
    ParticlesManifest.h
    SourceManager.cpp
    SourceManager.h
    StateChecker.cpp
    StateChecker.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Tests PRIVATE
    AppState.cpp
    AppState.h
    CollectionHelper.cpp
    CollectionHelper.h
    Event.cpp
    Event.h
    Figure.cpp
    Figure.h
    FigureCollection.cpp
    FigureCollection.h
    Particle.cpp
    Particle.h
    ParticleCollection.cpp
    ParticleCollection.h
    StateChecker.cpp
    StateChecker.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
