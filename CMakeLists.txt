cmake_minimum_required(VERSION 3.20)
project(AlgorithmicAlgebraicParser VERSION 1.6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define include directories
include_directories(include)

# ━━ CUSTOM FUNCTIONS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Helper function to create a module with standard properties
function(add_project_module TARGET_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES LINK_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_library(${TARGET_NAME} ${ARG_SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(ARG_LINK_LIBS)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ARG_LINK_LIBS})
    endif()
endfunction()

# Helper function to create and register a unit test
function(add_project_test TEST_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES LINK_LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${TEST_NAME} ${ARG_SOURCES})

    # Notice gtest_main is automatically linked here so we don't need main() in our cpp files
    target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main ${ARG_LINK_LIBS})

    include(GoogleTest)
    gtest_discover_tests(${TEST_NAME})
endfunction()

# ━━ LIBRARIES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

add_project_module(RegistryLib
        SOURCES
        src/reg/registry.cpp
        src/reg/math.cpp
)

add_project_module(CLILib
        SOURCES
        src/cli/cli.cpp
)

add_project_module(LexerLib
        SOURCES
        src/lexer/lexer.cpp
)

# ━━ EXECUTABLE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

add_executable(ParserMain src/main.cpp)
target_link_libraries(ParserMain PRIVATE RegistryLib CLILib LexerLib
)

# Cross-platform compiler flags
if(MSVC)
    target_compile_options(ParserMain PRIVATE /W4 /WX)
else()
    target_compile_options(ParserMain PRIVATE -Wall -Wextra -Werror)
endif()

# ━━ GTEST ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Enable CTest support in your IDE/Terminal
enable_testing()

# ━━ YOUR TESTS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Define the Registry tests
add_project_test(RegistryTests
        SOURCES tests/test_registry.cpp
        LINK_LIBS RegistryLib
)