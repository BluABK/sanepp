cmake_minimum_required(VERSION 3.14)
project(sane)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/cli)
include_directories(src/core)
include_directories(src/core/nlohmann)
include_directories(src/core/sapi_objs)
include_directories(src/gui)

add_executable(sane
        src/cli/main.cpp
        src/core/nlohmann/json.hpp
        src/core/sapi_objs/sapi_test_static_json.cpp
        src/core/sapi_objs/sapi_test_static_json.hpp
        src/core/api_handler.cpp
        src/core/api_handler.hpp
        src/core/main.cpp
        src/gui/main.cpp)
