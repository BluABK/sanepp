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

include_directories(src/cli)
include_directories(src/api_handler)
include_directories(src/third_party)
include_directories(src/api_handler/sapi_objs)
include_directories(src/gui)

# Database
include_directories(sanepp_rusty_db/src)
include_directories(sanepp_rusty_db/target/debug)

# Build required libs
execute_process(
        COMMAND /bin/bash -c ${CMAKE_SOURCE_DIR}/build-libs.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
include_directories(lib/sanepp_rusty_db/debug)

# CLI
add_executable(SaneCLI
        src/cli/main.cpp
        src/third_party/nlohmann/json.hpp
        src/api_handler/sapi_objs/sapi_test_static_json.cpp
        src/api_handler/sapi_objs/sapi_test_static_json.hpp
        src/api_handler/api_handler.cpp
        src/api_handler/api_handler.hpp
        src/api_handler/youtube_subscription.cpp
        src/api_handler/youtube_subscription.hpp
        src/cli/cli.cpp
        src/cli/cli.hpp
        sanepp_rusty_db/src/sanepp_rusty_db.h)

target_link_libraries (SaneCLI -lcurl)
target_link_libraries(SaneCLI ${CMAKE_SOURCE_DIR}/lib/sanepp_rusty_db/debug/libsanepp_rusty_db.so)

# GUI
add_executable(SaneGUI
        src/gui/main.cpp src/api_handler/youtube_subscription.cpp src/api_handler/youtube_subscription.hpp)


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
        src/third_party/nlohmann/json.hpp
        src/api_handler/sapi_objs/sapi_test_static_json.cpp
        src/api_handler/sapi_objs/sapi_test_static_json.hpp
        src/api_handler/api_handler.cpp
        src/api_handler/api_handler.hpp
        src/api_handler/youtube_subscription.cpp
        src/api_handler/youtube_subscription.hpp
        src/cli/cli.cpp
        src/cli/cli.hpp)

target_link_libraries(tests -lcurl)
target_link_libraries(tests Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(tests)