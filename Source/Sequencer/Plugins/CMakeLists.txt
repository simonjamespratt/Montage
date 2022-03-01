target_sources(Montage PRIVATE
    AutoParamGraph.cpp
    AutoParamGraph.h
    AutoParamValueSource.cpp
    AutoParamValueSource.h
    PluginsModal.cpp
    PluginsModal.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
