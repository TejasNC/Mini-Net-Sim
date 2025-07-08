cmake_minimum_required(VERSION 3.14)
project(NetworkSimulator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/simulator.cpp
    src/event.cpp
    src/node.cpp
    src/router.cpp
    src/interface.cpp
    src/link.cpp
    src/packets/packet.cpp
    src/packets/payload.cpp
)

# Create library
add_library(NetworkSimLib ${SOURCES})

# Main executable
add_executable(NetworkSimulator main.cpp)
target_link_libraries(NetworkSimulator NetworkSimLib)

# Compiler-specific options
if(MSVC)
    target_compile_options(NetworkSimLib PRIVATE /W4)
    target_compile_options(NetworkSimulator PRIVATE /W4)
else()
    target_compile_options(NetworkSimLib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(NetworkSimulator PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)
