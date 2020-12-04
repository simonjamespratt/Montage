target_sources(Montage PRIVATE
    Particles.cpp
    Particles.h
    ParticleSelector.cpp
    ParticleSelector.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
