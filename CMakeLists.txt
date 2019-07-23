cmake_minimum_required(VERSION 3.14)    # CMake version check

##
## PROJECT
## name and version
##
project(Sane++)                         # Create project
set(CMAKE_CXX_STANDARD 14)              # Enable c++14 standard


##
## OPTIONS
##
option(SanePP_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)


# DEBUG Flags, TODO: Figure out some RELEASE flags.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

include_directories(cli)
include_directories(api_handler)
include_directories(third_party)
include_directories(api_handler/src/sapi_objs)
include_directories(gui)

# CLI
add_executable(SaneCLI
        cli/src/main.cpp
        third_party/nlohmann/json.hpp
        api_handler/src/sapi_objs/sapi_test_static_json.cpp
        api_handler/src/sapi_objs/sapi_test_static_json.hpp
        api_handler/src/api_handler.cpp
        api_handler/src/api_handler.hpp
        api_handler/src/youtube_subscription.cpp
        api_handler/src/youtube_subscription.hpp
        cli/src/cli.cpp
        cli/src/cli.hpp)

target_link_libraries (SaneCLI -lcurl)

# GUI
add_executable(SaneGUI
        gui/src/main.cpp api_handler/src/youtube_subscription.cpp api_handler/src/youtube_subscription.hpp)


##
## TESTS
## create and configure the unit test target
##
#include(CTest) # adds option BUILD_TESTING (default ON)
#
#if(BUILD_TESTING AND SanePP_BuildTests)
#    enable_testing()
#
#    # Add the entire test/ directory
#    add_subdirectory(test)
#
#    # Link local libraries
#    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#endif()


enable_testing()
find_package(Catch2 REQUIRED)
add_executable(tests test/src/test.cpp
        test/src/test.cpp
        test/src/cli/unit-test_cli.cpp
        third_party/nlohmann/json.hpp
        api_handler/src/sapi_objs/sapi_test_static_json.cpp
        api_handler/src/sapi_objs/sapi_test_static_json.hpp
        api_handler/src/api_handler.cpp
        api_handler/src/api_handler.hpp
        api_handler/src/youtube_subscription.cpp
        api_handler/src/youtube_subscription.hpp
        cli/src/cli.cpp
        cli/src/cli.hpp)

target_link_libraries(tests -lcurl)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(tests)