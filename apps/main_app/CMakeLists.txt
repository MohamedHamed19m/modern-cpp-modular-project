# ============================================================================
# Main Application - Demonstrates Transitive Dependency
# ============================================================================
# This demonstrates the power of Modern CMake:
# - We only link to MathEngine::math_engine
# - We get logger AUTOMATICALLY (transitive via PUBLIC)
# - No need to manually specify include directories or other dependencies
# ============================================================================

# Collect source files
set(MAIN_APP_SOURCES
    main.cpp
)

# Create the executable target
add_executable(main_app ${MAIN_APP_SOURCES})

# ============================================================================
# Link Libraries - Transitive Dependency Demonstration
# ============================================================================
# Notice that we ONLY link to MathEngine::math_engine
# We do NOT need to link to MathEngine::logger explicitly!
#
# This works because in math_engine's CMakeLists.txt, we used:
#   target_link_libraries(math_engine PUBLIC MathEngine::logger)
#
# The PUBLIC keyword means:
#   1. math_engine can use logger
#   2. Anyone who links to math_engine also gets logger
# ============================================================================

target_link_libraries(main_app
    PRIVATE
        MathEngine::math_engine  # This brings in MathEngine::logger automatically!
)

# ============================================================================
# Explanation of PRIVATE here
# ============================================================================
# We use PRIVATE because:
#   - main_app is an executable, not a library
#   - No one else will link to main_app
#   - The dependency is only needed for main_app's compilation
#
# If main_app were a library that others link to, we would consider:
#   - PUBLIC:   if our headers exposed math_engine types
#   - PRIVATE:  if we only use it internally in .cpp files
# ============================================================================

# ============================================================================
# Platform-Specific Settings
# ============================================================================
if(MSVC)
    target_compile_options(main_app
        PRIVATE
            /W4
    )
else()
    target_compile_options(main_app
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

# ============================================================================
# IDE Folder Organization
# ============================================================================
set_target_properties(main_app PROPERTIES
    FOLDER "Applications"
)

message(STATUS "Main App: Configured (demonstrates transitive dependency)")
message(STATUS "  - Links to: MathEngine::math_engine")
message(STATUS "  - Gets automatically: MathEngine::logger (via PUBLIC)")
