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

include_directories(libsane++/src)
include_directories(libsane++/include)
include_directories(cli)
include_directories(gui)
include_directories(third_party)
include_directories(tests)

## Shared Libs
# Shared resources // FIXME: Import the local lib libsane++ the proper way.
#find_library(libsane++)
#add_library(libsane++ PATHS libsane++)

# ICU/Unicode
# ICU Components Reference:
# https://cmake.org/cmake/help/latest/module/FindICU.html
# ICU components = data, i18n, io, le, lx, test, tu and uc.
#find_package(ICU COMPONENTS uc REQUIRED)

# Command find_package in cmake doesn’t support SQLite.
# So, to link SQLite you have to go about it in a roundabout way...
set (LIB_DIR_LOCATION ${NAME}/lib)
find_library(sqlite3 NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})
set(INCLUDE_DIRS ${NAME}/include)

# CLI
add_executable(sane++_cli cli/src/main.cpp
        cli/src/cli.cpp
        cli/src/cli.hpp
        libsane++/src/api_handler/api_handler.cpp
        libsane++/include/api_handler/api_handler.hpp
        libsane++/src/entities/youtube_channel.cpp
        libsane++/include/entities/youtube_channel.hpp
        libsane++/src/db_handler/db_handler.cpp
        libsane++/include/db_handler/db_handler.hpp
        libsane++/src/db_handler/db_youtube_channels.cpp
        libsane++/include/db_handler/db_youtube_channels.hpp
        third_party/nlohmann/json.hpp
        libsane++/src/api_handler/entity_response.cpp
        libsane++/src/api_handler/json_response.cpp
        cli/src/entity_commands.cpp
        cli/src/json_commands.cpp)

# API Handler
target_link_libraries (sane++_cli -lcurl)
# Database
target_link_libraries(sane++_cli -lpthread)
target_link_libraries(sane++_cli -ldl)
target_link_libraries(sane++_cli sqlite3)
#target_link_libraries(sane++_cli ICU::uc)
target_include_directories(sane++_cli PRIVATE ${INCLUDE_DIRS})

# GUI
add_executable(sane++_gui gui/src/main.cpp
        libsane++/src/entities/youtube_channel.cpp
        libsane++/include/entities/youtube_channel.hpp
        libsane++/src/db_handler/db_handler.cpp
        libsane++/include/db_handler/db_handler.hpp
        libsane++/src/db_handler/db_youtube_channels.cpp
        libsane++/include/db_handler/db_youtube_channels.hpp
        libsane++/src/api_handler/api_handler.cpp
        libsane++/include/api_handler/api_handler.hpp
        libsane++/src/api_handler/entity_response.cpp
        libsane++/src/api_handler/json_response.cpp
        )

# API Handler
target_link_libraries (sane++_gui -lcurl)
# Database
target_link_libraries(sane++_gui -lpthread)
target_link_libraries(sane++_gui -ldl)
#target_link_libraries (SaneGUI ${SQLITE3_LIBRARIES})
target_link_libraries(sane++_gui sqlite3)
#target_link_libraries(sane++_gui ICU::uc)
target_include_directories(sane++_gui PRIVATE ${INCLUDE_DIRS})

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

if(DEFINED ENV{THIS_IS_TRAVIS})
    message("Building tests for Travis CI env...")

    add_executable(travis_test_all tests/test_all.cpp
            cli/src/cli.cpp
            cli/src/cli.hpp
            cli/test/unit-test_cli.cpp
            libsane++/src/api_handler/api_handler.cpp
            libsane++/include/api_handler/api_handler.hpp
            libsane++/src/entities/youtube_channel.cpp
            libsane++/include/entities/youtube_channel.hpp
            libsane++/src/db_handler/db_handler.cpp
            libsane++/include/db_handler/db_handler.hpp
            libsane++/test/db_handler/unit-test_exec_sql_statement_default_callback.cpp
            libsane++/src/db_handler/db_youtube_channels.cpp
            libsane++/include/db_handler/db_youtube_channels.hpp
            third_party/nlohmann/json.hpp
            libsane++/test/db_handler/unit-test_001_create_table_youtube_channels.cpp
            libsane++/test/entities/unit-test_001_create_youtube_channel_from_map.cpp
            libsane++/test/entities/unit-test_002_create_youtube_channel_from_json.cpp
            libsane++/src/api_handler/entity_response.cpp
            libsane++/src/api_handler/json_response.cpp
            cli/src/entity_commands.cpp
            cli/src/json_commands.cpp)

    # API Handler
    target_link_libraries(travis_test_all -lcurl)
    # Testing suite.
    target_link_libraries(travis_test_all Catch2::Catch2)
    # Database.
    target_link_libraries(travis_test_all -lpthread)
    target_link_libraries(travis_test_all -ldl)
    target_link_libraries(travis_test_all sqlite3)
#    target_link_libraries(travis_test_all ICU::uc)
    target_include_directories(travis_test_all PRIVATE ${INCLUDE_DIRS})

    include(CTest)
    include(Catch)

    catch_discover_tests(travis_test_all)
else()
    message("Building tests for normal env...")

    add_executable(test_all tests/test_all.cpp
            cli/src/cli.cpp
            cli/src/cli.hpp
            cli/test/unit-test_cli.cpp
            libsane++/src/api_handler/api_handler.cpp
            libsane++/include/api_handler/api_handler.hpp
            libsane++/src/entities/youtube_channel.cpp
            libsane++/include/entities/youtube_channel.hpp
            libsane++/src/db_handler/db_handler.cpp
            libsane++/include/db_handler/db_handler.hpp
            libsane++/test/db_handler/unit-test_exec_sql_statement_default_callback.cpp
            libsane++/src/db_handler/db_youtube_channels.cpp
            libsane++/include/db_handler/db_youtube_channels.hpp
            third_party/nlohmann/json.hpp
            libsane++/test/db_handler/unit-test_001_create_table_youtube_channels.cpp
            libsane++/test/db_handler/unit-test_002_add_channels.cpp
            libsane++/test/entities/unit-test_001_create_youtube_channel_from_map.cpp
            libsane++/test/entities/unit-test_002_create_youtube_channel_from_json.cpp
            libsane++/src/api_handler/entity_response.cpp
            libsane++/src/api_handler/json_response.cpp
            cli/src/entity_commands.cpp
            cli/src/json_commands.cpp)

    # API Handler
    target_link_libraries(test_all -lcurl)
    # Testing suite.
    target_link_libraries(test_all Catch2::Catch2)
    # Database.
    target_link_libraries(test_all -lpthread)
    target_link_libraries(test_all -ldl)
    target_link_libraries(test_all sqlite3)
#    target_link_libraries(test_all ICU::uc)
    target_include_directories(test_all PRIVATE ${INCLUDE_DIRS})

    include(CTest)
    include(Catch)

    catch_discover_tests(test_all)
endif()
