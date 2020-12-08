target_sources(Montage PRIVATE
    FigureGenerator.cpp
    FigureGenerator.h
    MyTestClass.cpp
    MyTestClass.h
    TestIdentifiers.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Tests PRIVATE
    MyTestClass.h
    MyTestClass.cpp
    TestIdentifiers.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
