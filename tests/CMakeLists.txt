# ============================================================================
# Tests - Demonstrating FetchContent for External Dependencies
# ============================================================================
# This demonstrates how to download and use external test frameworks
# without requiring manual installation or system-wide dependencies
# ============================================================================

# ============================================================================
# Fetch Catch2 Test Framework
# ============================================================================
# Catch2 is a modern, header-only test framework for C++
# We use FetchContent to download it automatically during configuration
#
# This is better than:
# - Requiring users to install Catch2 system-wide
# - Including vendored copy in repo (bloats git)
# - Assuming it's in some specific directory
# ============================================================================

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)

# Make Catch2 available (header-only for v3.x)
FetchContent_MakeAvailable(Catch2)

# ============================================================================
# Test Sources
# ============================================================================
set(TEST_SOURCES
    test_math.cpp
)

# ============================================================================
# Create Test Executable
# ============================================================================
# Note: We don't create a main() because Catch2 provides it
# We link to Catch2::Catch2WithMain which includes main()
# ============================================================================

add_executable(test_math ${TEST_SOURCES})

# Link to Catch2 (with main) and the library under test
target_link_libraries(test_math
    PRIVATE
        Catch2::Catch2WithMain
        MathEngine::math_engine  # Links to both math_engine AND logger
)

# ============================================================================
# IDE Folder Organization
# ============================================================================
set_target_properties(test_math PROPERTIES
    FOLDER "Tests"
)

# ============================================================================
# Register Tests with CTest
# ============================================================================
# add_test() registers the test with CTest
# ctest (command line tool) can then run all registered tests
# ============================================================================

include(CTest)

add_test(
    NAME MathTests
    COMMAND test_math
)

# Enable output on failure (helps debug)
set_tests_properties(MathTests PROPERTIES
    ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1"
)

message(STATUS "Tests: Configured with Catch2 (via FetchContent)")
message(STATUS "  - Test executable: test_math")
message(STATUS "  - Registered with CTest")
