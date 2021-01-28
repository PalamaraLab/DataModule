# This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
# See accompanying LICENSE and COPYING for copyright notice and full details.

find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Found Eigen3 ${Eigen3_VERSION}")

find_package(fmt CONFIG REQUIRED)
message(STATUS "Found {fmt} ${fmt_VERSION}")

find_package(ZLIB REQUIRED)
message(STATUS "Found zlib ${ZLIB_VERSION_STRING}")

set(
        data_module_src
        Utils.cpp
)

set(
        data_module_hdr
        EigenTypes.hpp
        Utils.hpp
)

add_library(data_module_lib STATIC ${data_module_src} ${data_module_hdr})
target_include_directories(data_module_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(data_module_lib PRIVATE Eigen3::Eigen fmt::fmt ZLIB::ZLIB)
target_link_libraries(data_module_lib PRIVATE project_warnings project_settings)
