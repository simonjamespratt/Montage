target_sources(Montage PRIVATE
    ErrorMessageModal.cpp
    ErrorMessageModal.h
    ErrorTypes.cpp
    ErrorTypes.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(Tests PRIVATE
    ErrorTypes.cpp
    ErrorTypes.h
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
