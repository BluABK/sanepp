cmake_minimum_required(VERSION 3.14)    # CMake version check
project(Sane++)                         # Create project
set(CMAKE_CXX_STANDARD 14)              # Enable c++14 standard

# DEBUG Flags, TODO: Figure out some RELEASE flags.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(src/cli)
include_directories(src/api_handler)
include_directories(src/third_party)
include_directories(src/api_handler/sapi_objs)
include_directories(src/gui)

# API (was Core)
#add_executable(SaneAPIHandler
#        src/api_handler/nlohmann/json.hpp
#        src/api_handler/sapi_objs/sapi_test_static_json.cpp
#        src/api_handler/sapi_objs/sapi_test_static_json.hpp
#        src/api_handler/api_handler.cpp
#        src/api_handler/api_handler.hpp
#        src/cli/main.cpp)

#target_link_libraries (SaneAPIHandler -lcurl)

# CLI
add_executable(SaneCLI
        src/cli/main.cpp
        src/third_party/nlohmann/json.hpp
        src/api_handler/sapi_objs/sapi_test_static_json.cpp
        src/api_handler/sapi_objs/sapi_test_static_json.hpp
        src/api_handler/api_handler.cpp
        src/api_handler/api_handler.hpp
        src/api_handler/youtube_subscription.cpp
        src/api_handler/youtube_subscription.hpp)

target_link_libraries (SaneCLI -lcurl)

# GUI
add_executable(SaneGUI
        src/gui/main.cpp src/api_handler/youtube_subscription.cpp src/api_handler/youtube_subscription.hpp)
