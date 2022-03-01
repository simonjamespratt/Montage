target_sources(Montage PRIVATE
    AudioRenderer.cpp
    AudioRenderer.h
    Icons.cpp
    Icons.h
    Identifiers.h
    DoubleValueEditorWithLabel.cpp
    DoubleValueEditorWithLabel.h
    DecisionModal.h
    DecisionModal.cpp
    MontageTracktionUIBehaviour.cpp
    MontageTracktionUIBehaviour.h
    NumericCollectionEditor.cpp
    NumericCollectionEditor.h
    NumericValueEditorWithLabel.cpp
    NumericValueEditorWithLabel.h
    SliderWithLabel.cpp
    SliderWithLabel.h
    StringEditorWithLabel.cpp
    StringEditorWithLabel.h
    Helpers.h
)

target_include_directories(Montage
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(Tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
