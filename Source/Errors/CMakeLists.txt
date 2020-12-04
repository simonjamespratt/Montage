target_sources(Montage PRIVATE
    ErrorManager.cpp
    ErrorManager.h
    ErrorMessage.cpp
    ErrorMessage.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
