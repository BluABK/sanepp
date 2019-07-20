cmake_minimum_required (VERSION 3.8)
project (Sane++ VERSION "0.0.0")

# DEBUG Flags, TODO: Figure out some RELEASE flags.
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${CMAKE_SOURCE_DIR}/config.hpp.in"
  "${CMAKE_BINARY_DIR}/config.hpp"
  )

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the binary tree to the search path for include files so that 
# we will find config.h
include_directories("${CMAKE_BINARY_DIR}" gui database core)

set(SOURCES_CORE	core/main.cpp core/api_handler.cpp core/sapi_objs/sapi_test_static_json.cpp)
set(SOURCES_GUI		gui/main.cpp)
set(SOURCES_CLI		cli/main.cpp)


# Add 1st party libraries
#include_directories("${PROJECT_SOURCE_DIR}/api")
#add_subdirectory (api)

# Add 3rd party libraries
#find_package(libCurl4 REQUIRED)
 
# Add the executables
# Core
add_executable(Sane++ ${SOURCES_CORE})
target_link_libraries (Sane++ -lcurl)
#install(TARGETS sane_core DESTINATION bin)
