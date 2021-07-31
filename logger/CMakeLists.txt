# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Ignore warnings from compiler.
add_definitions(-w)

# Set the project name
project (logger)

# Create a sources variable with a link to all cpp files to compile
# Not recommended.
set(SOURCES
    src/test.cpp
    src/logger.cpp
    src/log_formatter.cpp
)

# Add an executable with the above sources
add_executable(logger ${SOURCES})

add_subdirectory(3rd/fmt EXCLUDE_FROM_ALL)
target_link_libraries(logger fmt-header-only)

find_package(Threads)
target_link_libraries(logger ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenCV REQUIRED)
target_link_libraries(logger ${OpenCV_LIBS})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(logger
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/3rd
)