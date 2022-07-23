cmake_minimum_required(VERSION 3.10)

# DEGBUG MODE
set(CMAKE_BUILD_TYPE Debug)

# Project name
project(mstdlib LANGUAGES C)

# Set C version
set(CMAKE_C_STANDARD 11)

add_subdirectory(src)

# Specify Executable
add_executable(mstdlibUnitTests src/unit_tests/runner.c)

# Specify Library
set(SOURCE_FILES src/vector.c src/string.c src/hashmap.c)
add_library(mstdlib STATIC ${SOURCE_FILES})

# Link Libraries
target_link_libraries(mstdlibUnitTests VectorFunctions StringFunctions HashMapFunctions)
install(TARGETS mstdlibUnitTests DESTINATION ${CMAKE_INSTALL_PREFIX})

# Specify include directory
target_include_directories(mstdlibUnitTests PUBLIC 
    "${PROJECT_SOURCE_DIR}/includes"
    "${PROJECT_SOURCE_DIR}/src"
    )

