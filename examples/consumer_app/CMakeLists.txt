# ============================================================================
# Consumer App - Demonstrates find_package() Usage
# ============================================================================
# This example shows how to use MathEngine as an installed package:
# - Uses find_package() instead of add_subdirectory()
# - Links to imported target MathEngine::math_engine
# - Works after installing MathEngine to a prefix
# ============================================================================

cmake_minimum_required(VERSION 3.20)

# Small project for the example
project(ConsumerApp LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# Find the installed MathEngine package
# ============================================================================
# This requires:
#   1. MathEngine must be installed first (cmake --install build)
#   2. CMAKE_PREFIX_PATH must point to the install location
#      Or: set MathEngine_DIR to the cmake directory
#
# Example:
#   cmake -B build -S . -DCMAKE_PREFIX_PATH=/path/to/install
#   OR
#   cmake -B build -S . -DMathEngine_DIR=/path/to/install/lib/cmake/MathEngine
# ============================================================================

find_package(MathEngine REQUIRED)

# Create the executable
add_executable(consumer_app main.cpp)

# ============================================================================
# Link to MathEngine
# ============================================================================
# Notice we use the ALIAS target MathEngine::math_engine
# This target is "imported" - created by find_package()
#
# The imported target brings:
# - Include directories (automatically)
# - C++ standard requirements
# - Transitive dependencies (logger)
# - All compiler/linker settings needed
# ============================================================================

target_link_libraries(consumer_app
    PRIVATE
        MathEngine::math_engine
)

message(STATUS "Consumer App: Configured with installed MathEngine")
message(STATUS "  - Using find_package(MathEngine REQUIRED)")
message(STATUS "  - Links to imported target: MathEngine::math_engine")
