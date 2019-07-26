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
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wno-unknown-pragmas")

include_directories(libsane++/src)
include_directories(libsane++/include)
include_directories(cli)
include_directories(db_handler/src)
include_directories(gui)
include_directories(third_party)
include_directories(tests)

# Shared resources // FIXME: Import the local lib libsane++ the proper way.
#find_library(libsane++)
#add_library(libsane++ PATHS libsane++)

# CLI
add_executable(SaneCLI
        cli/src/main.cpp
        third_party/nlohmann/json.hpp
        libsane++/src/api_handler/api_handler.cpp
        libsane++/include/api_handler/api_handler.hpp
        libsane++/src/entities/youtube_channel.cpp
        libsane++/src/entities/youtube_channel.hpp
        cli/src/cli.cpp
        cli/src/cli.hpp db_handler/src/db_handler.cpp
        db_handler/src/db_handler.hpp
        third_party/sqlite/sqlite3.h
        third_party/sqlite/sqlite3.c
        db_handler/src/db_youtube_channels.cpp
        db_handler/src/db_youtube_channels.hpp)

# API Handler
target_link_libraries (SaneCLI -lcurl)
# Database
target_link_libraries(SaneCLI -lpthread)
target_link_libraries(SaneCLI -ldl)


# GUI
add_executable(SaneGUI
        gui/src/main.cpp
        libsane++/src/entities/youtube_channel.cpp
        libsane++/src/entities/youtube_channel.hpp
        db_handler/src/db_handler.cpp
        db_handler/src/db_handler.hpp
        third_party/sqlite/sqlite3.h
        third_party/sqlite/sqlite3.c
        db_handler/src/db_youtube_channels.cpp
        db_handler/src/db_youtube_channels.hpp)

# API Handler
target_link_libraries (SaneGUI -lcurl)
# Database
target_link_libraries(SaneGUI -lpthread)
target_link_libraries(SaneGUI -ldl)

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
add_executable(test_all tests/test_all.cpp
        cli/test/unit-test_cli.cpp
        third_party/nlohmann/json.hpp
        libsane++/src/api_handler/api_handler.cpp
        libsane++/include/api_handler/api_handler.hpp
        libsane++/src/entities/youtube_channel.cpp
        libsane++/src/entities/youtube_channel.hpp
        cli/src/cli.cpp
        cli/src/cli.hpp
        db_handler/src/db_handler.cpp
        db_handler/src/db_handler.hpp
        third_party/sqlite/sqlite3.h
        third_party/sqlite/sqlite3.c
        db_handler/test/unit-test_exec_sql_statement_default_callback.cpp
        db_handler/src/db_youtube_channels.cpp
        db_handler/src/db_youtube_channels.hpp)

# API Handler
target_link_libraries(test_all -lcurl)
# Testing suite.
target_link_libraries(test_all Catch2::Catch2)
# Database.
target_link_libraries(test_all -lpthread)
target_link_libraries(test_all -ldl)

include(CTest)
include(Catch)

catch_discover_tests(test_all)