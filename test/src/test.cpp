// In a Catch project with multiple files, dedicate one file to compile the
// source code of Catch itself and reuse the resulting object file for linking.

#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

TEST_CASE( "1: All test cases reside in other .cpp files (empty)", "[multi-file:1]" ) {}

// ^^^
// Normally no TEST_CASEs in this file.
// Here just to show there are two source files via option --list-tests.