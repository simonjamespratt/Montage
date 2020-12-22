target_sources(Montage PRIVATE
    Icons.cpp
    Icons.h
    Identifiers.h
    NumericCollectionEditor.cpp
    NumericCollectionEditor.h
    NumericValueEditorWithLabel.cpp
    NumericValueEditorWithLabel.h
    SliderWithLabel.cpp
    SliderWithLabel.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
