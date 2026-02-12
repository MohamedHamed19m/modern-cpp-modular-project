# ============================================================================
# Math Engine Library - Compiled STATIC Library
# ============================================================================
# This demonstrates a compiled library pattern:
# - Source files are compiled into a static library
# - PUBLIC dependency on logger (transitive to consumers)
# - Consumers of math_engine automatically get logger
# ============================================================================

# Collect source files
set(MATH_ENGINE_SOURCES
    src/calculator.cpp
)

set(MATH_ENGINE_HEADERS
    include/math/calculator.hpp
)

# Create the STATIC library target
add_library(math_engine STATIC ${MATH_ENGINE_SOURCES} ${MATH_ENGINE_HEADERS})

# Alias target for namespaced convention (Modern CMake pattern)
add_library(MathEngine::math_engine ALIAS math_engine)

# ============================================================================
# Include Directories with Generator Expressions
# ============================================================================
target_include_directories(math_engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# ============================================================================
# C++ Standard Requirement
# ============================================================================
target_compile_features(math_engine
    PUBLIC
        cxx_std_20
)

# ============================================================================
# Dependencies - Demonstrating Visibility
# ============================================================================
# PUBLIC:    Math_engine needs it AND consumers need it (transitive)
# PRIVATE:   Math_engine needs it BUT consumers don't (implementation detail)
# INTERFACE: Math_engine doesn't need it BUT consumers do (header-only)
# ============================================================================

# Link to logger with PUBLIC visibility
# This means: consumers of math_engine also get access to logger
# (transitive dependency - key Modern CMake concept!)
target_link_libraries(math_engine
    PUBLIC
        MathEngine::logger
)

# Explanation of why PUBLIC is used here:
# - calculator.hpp includes logger/logger.hpp (in public headers)
# - Therefore, consumers of math_engine also need logger headers
# - If logger was only used in .cpp files, this would be PRIVATE

# ============================================================================
# Platform-Specific Settings
# ============================================================================
if(MSVC)
    target_compile_options(math_engine
        PRIVATE
            /W4
            /WX  # Treat warnings as errors
    )
else()
    target_compile_options(math_engine
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror  # Treat warnings as errors
    )
endif()

# ============================================================================
# IDE Folder Organization (Visual Studio, Xcode)
# ============================================================================
set_target_properties(math_engine PROPERTIES
    FOLDER "Libraries"
)

# ============================================================================
# Installation & Export
# ============================================================================
if(ENABLE_INSTALL)
    include(GNUInstallDirs)

    # Install the library (compiled artifact)
    install(
        TARGETS math_engine
        EXPORT MathEngineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install headers
    install(
        DIRECTORY
            include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h"
    )

    message(STATUS "MathEngine: Static library configured")
    message(STATUS "  - Transitive dependency: MathEngine::logger (PUBLIC)")
endif()
