# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(mazing-engine VERSION 1.0 LANGUAGES C)

# Find GLFW (adjust for manual installation or non-standard locations)
find_package(GLFW3 REQUIRED)
find_package(OpenCL REQUIRED)

if (NOT GLFW3_FOUND)
    message(FATAL_ERROR "GLFW not found! Please install GLFW and ensure it's in your PATH.")
endif()

if (NOT OpenCL_FOUND)
    message(FATAL_ERROR "OpenCL not found! Please install OpenCL and ensure it's in your PATH.")
endif()

# Define where to place compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Recursively collect all source files in src/
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Define common compile options
set(COMMON_FLAGS -Wall -Wextra -std=c99 -fdiagnostics-color=always)

# Debug target
add_executable(mazing-engine-debug ${SOURCES})
target_compile_options(mazing-engine-debug PRIVATE ${COMMON_FLAGS} -O0 -g)
set_target_properties(mazing-engine-debug PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
target_link_libraries(mazing-engine-debug ncurses m glfw OpenCL::OpenCL)

# Release target
add_executable(mazing-engine-release ${SOURCES})
target_compile_options(mazing-engine-release PRIVATE ${COMMON_FLAGS}
    -Ofast -march=native -flto -funroll-loops -fno-math-errno -fno-trapping-math
    -fprefetch-loop-arrays -ffp-contract=fast -ftree-vectorize
    -fsingle-precision-constant -fassociative-math)
set_target_properties(mazing-engine-release PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
target_link_libraries(mazing-engine-release ncurses m glfw OpenCL::OpenCL)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "Removing all build files."
)

# Handle GLFW path manually if it's installed in non-standard locations
if (NOT TARGET GLFW::GLFW)
    find_path(GLFW_INCLUDE_DIR glfw3.h)
    find_library(GLFW_LIBRARY NAMES glfw3)
    add_library(GLFW::GLFW STATIC IMPORTED)
    set_target_properties(GLFW::GLFW PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
    )
endif()
