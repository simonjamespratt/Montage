target_sources(Montage PRIVATE
    DurationProtocolController.h
    DurationProtocolController.cpp
    GeometricProtocolController.cpp
    GeometricProtocolController.h
    MultiplesProtocolController.cpp
    MultiplesProtocolController.h
    PrescribedProtocolController.h
    PrescribedProtocolController.cpp
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
