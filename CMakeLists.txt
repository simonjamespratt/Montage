# Based on this: https://github.com/juce-framework/JUCE/blob/master/examples/CMake/GuiApp/CMakeLists.txt
cmake_minimum_required(VERSION 3.15.0)
project(
    montage
    VERSION 0.1.0
    DESCRIPTION "An audio desktop application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies (external content)
add_subdirectory(dependencies)

# Main targets
add_subdirectory(source)
