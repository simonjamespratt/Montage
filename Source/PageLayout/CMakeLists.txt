target_sources(Montage PRIVATE
    ContentContainer.cpp
    ContentContainer.h
    Figures.cpp
    Figures.h
    MainHeader.cpp
    MainHeader.h
    ParticlesPage.cpp
    ParticlesPage.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
