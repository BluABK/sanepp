cmake_minimum_required(VERSION 3.5)

project(sane)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Gui Widgets Core)

find_program(CCACHE_FOUND cache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

include_directories(src src/gui src/database)

set(SOURCES_GUI
  src/gui/main.cpp
)

set(SOURCES_CLI
  src/cli/main.cpp
)

add_executable(sane_gui ${SOURCES_GUI} ${qml_QRC})
qt5_use_modules(sane_gui Qml Quick Core Gui Widgets)
install(TARGETS sane_gui DESTINATION bin)

add_executable(sane_cli ${SOURCES_GUI} ${qml_QRC})
qt5_use_modules(sane_cli Core Gui)
install(TARGETS sane_cli DESTINATION bin)

