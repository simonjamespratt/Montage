target_sources(Montage PRIVATE
    ParticlesSelection.cpp
    ParticlesSelection.h
    ParticleSelector.cpp
    ParticleSelector.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
