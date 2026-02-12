cmake_minimum_required(VERSION 3.20)

project(MathEngine
    VERSION 1.0.0
    DESCRIPTION "Modern CMake Mastery Project - A reference for CMake best practices"
    LANGUAGES CXX
)

# ============================================================================
# Project Options
# ============================================================================
option(BUILD_TESTING "Build the test suite" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(ENABLE_INSTALL "Enable install targets for find_package support" ON)

# ============================================================================
# C++ Standard Settings
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Global Settings
# ============================================================================
# Use folder-based structure in IDEs (Visual Studio/Xcode)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Export compile commands for IDEs (VS Code, CLion, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# External Dependencies via FetchContent
# ============================================================================
include(FetchContent)

# fmt library - modern formatting library (optional, for examples)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
# Only fetch if needed
FetchContent_MakeAvailable(fmt)

# ============================================================================
# Subdirectories
# ============================================================================
# Add sub-packages (internal libraries)
add_subdirectory(libs/logger)

add_subdirectory(libs/math_engine)

# Add applications (executables)
add_subdirectory(apps/main_app)

# Note: examples/consumer_app is NOT built here
# It is a standalone example that must be built separately after installing MathEngine:
#   cmake --install build --prefix /path/to/install
#   cmake -B build/consumer -S examples/consumer_app -DCMAKE_PREFIX_PATH=/path/to/install

# Add tests
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ============================================================================
# Installation & Export (for find_package support)
# ============================================================================
if(ENABLE_INSTALL)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    # Configure the version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MathEngineConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    # Configure the main config file
    configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake/MathEngineConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/MathEngineConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MathEngine
    )

    # Install the config files
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/MathEngineConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/MathEngineConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MathEngine
    )

    # Install exported targets
    install(
        EXPORT MathEngineTargets
        FILE MathEngineTargets.cmake
        NAMESPACE MathEngine::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MathEngine
    )

    message(STATUS "MathEngine: Install targets enabled")
    message(STATUS "  - Config: ${CMAKE_INSTALL_LIBDIR}/cmake/MathEngine")
    message(STATUS "  - Include: ${CMAKE_INSTALL_INCLUDEDIR}")
endif()

# ============================================================================
# Summary
# ============================================================================
message(STATUS "")
message(STATUS "==================== MathEngine Configuration ====================")
message(STATUS "  Version:     ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:  ${CMAKE_BUILD_TYPE}")
message(STATUS "  Testing:     ${BUILD_TESTING}")
message(STATUS "  Examples:    ${BUILD_EXAMPLES}")
message(STATUS "  Install:     ${ENABLE_INSTALL}")
message(STATUS "================================================================")
message(STATUS "")
