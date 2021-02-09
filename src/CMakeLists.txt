# This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
# See accompanying LICENSE and COPYING for copyright notice and full details.

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
        HapsMatrixType.cpp
        utils/FileUtils.cpp
        utils/StringUtils.cpp
)

set(
        data_module_hdr
        HapsMatrixType.hpp
        utils/EigenTypes.hpp
        utils/FileUtils.hpp
        utils/StringUtils.hpp
)

add_library(data_module_lib STATIC ${data_module_src} ${data_module_hdr})
target_include_directories(data_module_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(data_module_lib PRIVATE Eigen3::Eigen fmt::fmt range-v3 ZLIB::ZLIB)
target_link_libraries(data_module_lib PRIVATE project_warnings project_settings)

if (PYTHON_BINDINGS)
    set_target_properties(data_module_lib PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
    pybind11_add_module(asmc_data_module python_bindings/bindings.cpp)
    target_link_libraries(asmc_data_module PRIVATE Eigen3::Eigen fmt::fmt range-v3 ZLIB::ZLIB data_module_lib)
endif ()