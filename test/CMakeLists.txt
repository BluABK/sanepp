#option(SanePP_Valgrind "Execute test suite with Valgrind" OFF)
#
#find_package(Catch2 REQUIRED)
#add_executable(tests src/test.cpp)
#target_link_libraries(tests Catch2::Catch2)
#
#include(Catch)
#
#catch_discover_tests(tests)
#
#
##############################################################################
## one executable for each unit test file
##############################################################################
#
##add_executable(SaneCLI
##        src/cli/main.cpp
##        src/third_party/nlohmann/json.hpp
##        src/api_handler/sapi_objs/sapi_test_static_json.cpp
##        src/api_handler/sapi_objs/sapi_test_static_json.hpp
##        src/api_handler/api_handler.cpp
##        src/api_handler/api_handler.hpp
##        src/api_handler/youtube_subscription.cpp
##        src/api_handler/youtube_subscription.hpp
##        src/cli/cli.cpp
##        src/cli/cli.hpp)
#
