target_sources(Tests PRIVATE
    StateHelpers.cpp
    StateHelpers.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
