cmake_minimum_required(VERSION 3.20)
project(AlgorithmicAlgebraicParser VERSION 1.6)

# Enforce C++20 standard as per SAD [cite: 84, 205]
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define include directories so Intellisense can find your headers [cite: 76]
include_directories(include/reg)
include_directories(include/core)

# Collect source files for the Registry module [cite: 75]
set(REGISTRY_SOURCES
        src/reg/registry.cpp
        src/reg/standard_math.cpp
        # Add other .cpp files here as you create them
)

# Create the executable (eventually this will include all module sources)
add_executable(ParserMain
        src/main.cpp
        ${REGISTRY_SOURCES}
)

# Cross-platform compiler flags
if(MSVC)
    # Windows-specific flags (if using MSVC instead of LLVM)
    target_compile_options(ParserMain PRIVATE /W4 /WX)
else()
    # Linux/LLVM flags [cite: 70]
    target_compile_options(ParserMain PRIVATE -Wall -Wextra -Werror)
endif()