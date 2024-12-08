# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(mazing-engine VERSION 1.0 LANGUAGES C)

# Define where to place compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable all warnings and debugging
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Recursively collect all source files in src/
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link required libraries (e.g., ncurses)
target_link_libraries(${PROJECT_NAME} ncurses m)

# Add a custom clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "Removing all build files."
)
