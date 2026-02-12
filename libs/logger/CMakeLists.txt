# ============================================================================
# Logger Library - Header-Only INTERFACE Library
# ============================================================================
# This demonstrates an INTERFACE library pattern:
# - No source files are compiled
# - Headers are included directly by consumers
# - Usage requirements are propagated via INTERFACE properties
# ============================================================================

# Create the INTERFACE library target
add_library(logger INTERFACE)

# Alias target for namespaced convention (Modern CMake pattern)
add_library(MathEngine::logger ALIAS logger)

# ============================================================================
# Include Directories with Generator Expressions
# ============================================================================
# Generator expressions allow different behavior during build vs install:
# - BUILD_INTERFACE: Used when building this project directly
# - INSTALL_INTERFACE: Used when finding this package via find_package()
# ============================================================================
target_include_directories(logger
    INTERFACE
        # During build: use the source directory
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # After install: use the installed include directory
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ============================================================================
# C++ Standard Requirement
# ============================================================================
# Users of this library must have at least C++20 available
# ============================================================================
target_compile_features(logger
    INTERFACE
        cxx_std_20
)

# ============================================================================
# Platform-Specific Settings
# ============================================================================
if(MSVC)
    # Enable more warnings on MSVC
    target_compile_options(logger
        INTERFACE
            /W4
    )
else()
    # Enable more warnings on GCC/Clang
    target_compile_options(logger
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

# ============================================================================
# Installation & Export
# ============================================================================
# For header-only libraries, we only need to install headers
# ============================================================================
if(ENABLE_INSTALL)
    include(GNUInstallDirs)

    # Install headers to include/logger/
    install(
        DIRECTORY
            include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h"
    )

    # Export the target for find_package() support
    install(
        TARGETS logger
        EXPORT MathEngineTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    message(STATUS "Logger: Header-only library configured")
endif()
