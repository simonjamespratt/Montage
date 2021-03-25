target_sources(Montage PRIVATE
    ParticleEditor.cpp
    ParticleEditor.h
    ParticlesManager.cpp
    ParticlesManager.h
    ParticleView.cpp
    ParticleView.h
    SourceSelector.cpp
    SourceSelector.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
