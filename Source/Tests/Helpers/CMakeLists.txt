target_sources(Tests PRIVATE
    TestHelpers.cpp
    TestHelpers.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
