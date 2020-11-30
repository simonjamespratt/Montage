target_sources(Montage PRIVATE
    Icons.cpp
    Icons.h
    Identifiers.h
    NumberGenerators.cpp
    NumberGenerators.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
