cmake_minimum_required(VERSION 3.14)    # CMake version check
project(Sane++)                         # Create project
set(CMAKE_CXX_STANDARD 14)              # Enable c++14 standard

# DEBUG Flags, TODO: Figure out some RELEASE flags.
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(src/cli)
include_directories(src/core)
include_directories(src/core/nlohmann)
include_directories(src/core/sapi_objs)
include_directories(src/gui)

# Core
add_executable(SaneCore
        src/core/nlohmann/json.hpp
        src/core/sapi_objs/sapi_test_static_json.cpp
        src/core/sapi_objs/sapi_test_static_json.hpp
        src/core/api_handler.cpp
        src/core/api_handler.hpp
        src/core/main.cpp)

target_link_libraries (SaneCore -lcurl)

# CLI
add_executable(SaneCLI
        src/cli/main.cpp)
# GUI
add_executable(SaneGUI
        src/gui/main.cpp)
