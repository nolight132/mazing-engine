# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(mazing-engine VERSION 1.0 LANGUAGES C)

# Try to find GLFW using the system-wide installation
find_package(GLFW3 REQUIRED)

if (NOT GLFW3_FOUND)
    find_package(GLFW)
    if (NOT GLFW_FOUND)
        message(FATAL_ERROR "GLFW not found! Please install GLFW and ensure it's in your PATH.")
    endif()

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
target_compile_options(mazing-engine-debug PRIVATE ${COMMON_FLAGS} -g)
target_link_libraries(mazing-engine-debug ncurses m glfw)

# Release target
add_executable(mazing-engine-release ${SOURCES})
target_compile_options(mazing-engine-release PRIVATE ${COMMON_FLAGS}
    -Ofast -march=native -flto -funroll-loops -fno-math-errno -fno-trapping-math
    -fprefetch-loop-arrays -ffp-contract=fast -ftree-vectorize
    -fsingle-precision-constant -fassociative-math)
target_link_libraries(mazing-engine-release ncurses m glfw)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "Removing all build files."
)
