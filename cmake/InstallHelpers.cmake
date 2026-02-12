# ============================================================================
# InstallHelpers.cmake
# ============================================================================
# Reusable CMake functions for common installation tasks
# This module can be included in other projects
# ============================================================================

# ============================================================================
# Function: setup_target_warnings
# ============================================================================
# Sets up compiler warnings for a target
#
# Parameters:
#   TARGET - The target to configure
#   AS_ERRORS - (Optional) Treat warnings as errors (default: OFF)
#
# Usage:
#   setup_target_warnings(TARGET mylib AS_ERRORS ON)
# ============================================================================

function(setup_target_warnings)
    cmake_parse_arguments(ARGS
        ""  # No options
        "TARGET"  # Single-value arguments
        "AS_ERRORS"  # Multi-value arguments
        ${ARGN}
    )

    if(NOT ARGS_TARGET)
        message(FATAL_ERROR "setup_target_warnings: TARGET is required")
    endif()

    if(MSVC)
        list(APPEND WARNING_OPTS /W4)
        if(ARGS_AS_ERRORS)
            list(APPEND WARNING_OPTS /WX)
        endif()
    else()
        list(APPEND WARNING_OPTS -Wall -Wextra -Wpedantic)
        if(ARGS_AS_ERRORS)
            list(APPEND WARNING_OPTS -Werror)
        endif()
    endif()

    target_compile_options(${ARGS_TARGET} PRIVATE ${WARNING_OPTS})

    message(VERBOSE "Configured warnings for ${ARGS_TARGET}")
endfunction()

# ============================================================================
# Function: install_target_with_headers
# ============================================================================
# Installs a library target and its headers with proper export
#
# Parameters:
#   TARGET - The target to install
#   EXPORT_SET - The export set name
#   HEADER_DIR - Directory containing headers (relative to CMAKE_CURRENT_SOURCE_DIR)
#
# Usage:
#   install_target_with_headers(
#       TARGET mylib
#       EXPORT_SET MyProjectTargets
#       HEADER_DIR include
#   )
# ============================================================================

function(install_target_with_headers)
    cmake_parse_arguments(ARGS
        ""
        "TARGET;EXPORT_SET;HEADER_DIR"
        ""
        ${ARGN}
    )

    if(NOT ARGS_TARGET)
        message(FATAL_ERROR "install_target_with_headers: TARGET is required")
    endif()
    if(NOT ARGS_EXPORT_SET)
        message(FATAL_ERROR "install_target_with_headers: EXPORT_SET is required")
    endif()

    include(GNUInstallDirs)

    # Install the target
    install(
        TARGETS ${ARGS_TARGET}
        EXPORT ${ARGS_EXPORT_SET}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install headers if directory specified
    if(ARGS_HEADER_DIR)
        install(
            DIRECTORY ${ARGS_HEADER_DIR}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING
                PATTERN "*.hpp"
                PATTERN "*.h"
                PATTERN "*.hxx"
        )
    endif()

    message(VERBOSE "Installed target ${ARGS_TARGET} with headers")
endfunction()

# ============================================================================
# Function: create_alias_targets
# ============================================================================
# Creates namespaced alias targets for modern CMake convention
#
# Parameters:
#   NAMESPACE - The namespace prefix (e.g., "MyProject")
#   TARGETS - List of targets to create aliases for
#
# Usage:
#   create_alias_targets(
#       NAMESPACE MathEngine
#       TARGETS logger math_engine
#   )
#   # Creates: MathEngine::logger and MathEngine::math_engine
# ============================================================================

function(create_alias_targets)
    cmake_parse_arguments(ARGS
        ""
        "NAMESPACE"
        "TARGETS"
        ${ARGN}
    )

    if(NOT ARGS_NAMESPACE)
        message(FATAL_ERROR "create_alias_targets: NAMESPACE is required")
    endif()
    if(NOT ARGS_TARGETS)
        message(FATAL_ERROR "create_alias_targets: TARGETS is required")
    endif()

    foreach(target IN LISTS ARGS_TARGETS)
        add_library(${ARGS_NAMESPACE}::${target} ALIAS ${target})
        message(VERBOSE "Created alias ${ARGS_NAMESPACE}::${target}")
    endforeach()
endfunction()

# ============================================================================
# Function: configure_package_config_files
# ============================================================================
# Creates package configuration files for find_package() support
#
# Parameters:
#   PROJECT_NAME - Name of the project
#   VERSION - Project version (e.g., "1.0.0")
#   CONFIG_IN - Input .cmake.in file path
#   COMPATIBILITY - SameMajorVersion, SameMinorVersion, or AnyNewerVersion
#
# Usage:
#   configure_package_config_files(
#       PROJECT_NAME MyProject
#       VERSION ${PROJECT_VERSION}
#       CONFIG_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MyProjectConfig.cmake.in
#       COMPATIBILITY SameMajorVersion
#   )
# ============================================================================

function(configure_package_config_files)
    cmake_parse_arguments(ARGS
        ""
        "PROJECT_NAME;VERSION;CONFIG_IN;COMPATIBILITY"
        ""
        ${ARGN}
    )

    if(NOT ARGS_PROJECT_NAME)
        message(FATAL_ERROR "configure_package_config_files: PROJECT_NAME is required")
    endif()
    if(NOT ARGS_VERSION)
        message(FATAL_ERROR "configure_package_config_files: VERSION is required")
    endif()
    if(NOT ARGS_CONFIG_IN)
        message(FATAL_ERROR "configure_package_config_files: CONFIG_IN is required")
    endif()
    if(NOT ARGS_COMPATIBILITY)
        set(ARGS_COMPATIBILITY SameMajorVersion)
    endif()

    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    # Generate version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${ARGS_VERSION}
        COMPATIBILITY ${ARGS_COMPATIBILITY}
    )

    # Generate config file
    configure_package_config_file(
        ${ARGS_CONFIG_IN}
        "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ARGS_PROJECT_NAME}
    )

    # Install config files
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${ARGS_PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ARGS_PROJECT_NAME}
    )

    message(VERBOSE "Configured package files for ${ARGS_PROJECT_NAME}")
endfunction()
