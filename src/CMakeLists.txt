# This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
# See accompanying LICENSE and COPYING for copyright notice and full details.

add_subdirectory(third_party)

find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Found Eigen3 ${Eigen3_VERSION}")

find_package(fmt CONFIG REQUIRED)
message(STATUS "Found {fmt} ${fmt_VERSION}")

find_package(range-v3 CONFIG REQUIRED)
message(STATUS "Found range-v3 ${range-v3_VERSION}")
#target_link_libraries(main PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)

find_package(ZLIB REQUIRED)
message(STATUS "Found zlib ${ZLIB_VERSION_STRING}")

set(
        data_module_src
        BedMatrixType.cpp
        GeneticMap.cpp
        HapsMatrixType.cpp
        PlinkMap.cpp
        utils/FileUtils.cpp
        utils/StringUtils.cpp
)

set(
        data_module_hdr
        BedMatrixType.hpp
        GeneticMap.hpp
        HapsMatrixType.hpp
        PlinkMap.hpp
        EigenTypes.hpp
        utils/FileUtils.hpp
        utils/StringUtils.hpp
)

set(
        data_module_public_hdr
        ${CMAKE_CURRENT_SOURCE_DIR}/BedMatrixType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GeneticMap.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/HapsMatrixType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PlinkMap.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/EigenTypes.hpp
)

add_library(data_module_lib STATIC ${data_module_src} ${data_module_hdr})

target_include_directories(
        data_module_lib INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(data_module_lib PROPERTIES PUBLIC_HEADER "${data_module_public_hdr}")

target_link_libraries(data_module_lib PRIVATE Eigen3::Eigen fmt::fmt range-v3 ZLIB::ZLIB)
target_link_libraries(data_module_lib PRIVATE project_warnings project_settings)
target_link_libraries(data_module_lib PRIVATE pandas_plink_lib)

if (PYTHON_BINDINGS)
    set_target_properties(data_module_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    pybind11_add_module(asmc_data_module python_bindings/bindings.cpp)
    target_link_libraries(asmc_data_module PRIVATE Eigen3::Eigen fmt::fmt range-v3 ZLIB::ZLIB data_module_lib)
endif ()