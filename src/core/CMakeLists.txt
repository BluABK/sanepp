cmake_minimum_required (VERSION 3.8)
project (Sane++ CXX)

# The version number.
set (SanePP_VERSION "0.0.0")

# Set directories
set (PROJECT_SOURCE_DIR )

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# Add the binary tree to the search path for include files so that 
# we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

# Add 1st party libraries
#include_directories("${PROJECT_SOURCE_DIR}/api")
#add_subdirectory (api)

# Add 3rd party libraries
#find_package(libCurl4 REQUIRED)
 
# Add the executable
add_executable(Sane++ main.cpp)
target_link_libraries (Sane++ -lcurl)
