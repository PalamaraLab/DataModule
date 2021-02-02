# This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
# See accompanying LICENSE and COPYING for copyright notice and full details.


cmake_minimum_required(VERSION 3.16)
message(STATUS "Using CMake version ${CMAKE_VERSION}")
include(cmake/AutodetectVcpkgToolchainFile.cmake)

project(DataModule LANGUAGES CXX VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Link this 'library' to set the C++ standard / compile-time options requested
include(cmake/ProjectSettings.cmake)
add_library(project_settings INTERFACE)
target_compile_features(project_settings INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Sanitiser options if supported by compiler
include(cmake/Sanitisers.cmake)
enable_sanitisers(project_settings)

# allow for static analysis options
include(cmake/StaticAnalysers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

#option(PYTHON_BINDINGS "Whether to build the python bindings" ON)

add_subdirectory(src)

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()

option(MAKE_DOCS "Enable doxygen/sphinx commands" OFF)
if (EXISTS ${CMAKE_SOURCE_DIR}/venv/bin/sphinx-build)
    set(MAKE_DOCS ON)
endif()
if (MAKE_DOCS)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
    add_subdirectory(docs)
endif ()

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
if (ENABLE_UNITY)
    # Add for any project you want to apply unity builds for
    set_target_properties(intro PROPERTIES UNITY_BUILD ON)
endif ()
