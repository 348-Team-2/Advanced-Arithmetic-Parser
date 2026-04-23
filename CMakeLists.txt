cmake_minimum_required(VERSION 3.20)
project(AlgorithmicAlgebraicParser VERSION 1.6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define include directories
include_directories(include)

set(REGISTRY_SOURCES
        src/reg/registry.cpp
        src/reg/math.cpp
)

set(CLI_SOURCES
        src/cli/cli.cpp
)

# ━━ LIBRARIES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# Bundle the registry into a library so it can be shared between Main and Tests
add_library(RegistryLib ${REGISTRY_SOURCES})
target_include_directories(RegistryLib PUBLIC include)

add_library(CLILib ${CLI_SOURCES})
target_include_directories(CLILib PUBLIC include)

# ━━ EXECUTABLE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
add_executable(ParserMain src/main.cpp)

target_link_libraries(ParserMain PRIVATE RegistryLib CLILib)

# Cross-platform compiler flags
if(MSVC)
    # Windows-specific flags
    target_compile_options(ParserMain PRIVATE /W4 /WX)
else()
    # Linux/LLVM flags
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
add_executable(run_tests tests/test_registry.cpp)
target_link_libraries(run_tests
        PRIVATE
        gtest
        RegistryLib
)

# Enable CTest support in your IDE/Terminal
include(CTest)
add_test(NAME RegistryTests COMMAND run_tests)