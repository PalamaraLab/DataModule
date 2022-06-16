# This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
# See accompanying LICENSE and COPYING for copyright notice and full details.

Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_SHALLOW TRUE
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 605a34765aa5d5ecbf476b4598a862ada971b0cc # version 3.0.1
)
FetchContent_MakeAvailable(Catch2)
message(STATUS "Obtained Catch2 ${Catch2_VERSION}")

find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Found Eigen3 ${Eigen3_VERSION}")

find_package(fmt CONFIG REQUIRED)
message(STATUS "Found {fmt} ${fmt_VERSION}")

add_definitions(-DDATA_MODULE_TEST_DIR=\"${CMAKE_CURRENT_LIST_DIR}\")

set(
        test_src
        TestBedMatrixType.cpp
        TestGeneticMap.cpp
        TestHapsMatrixType.cpp
        TestPlinkMap.cpp
        utils/TestFileUtils.cpp
        utils/TestStringUtils.cpp
        utils/TestVectorUtils.cpp
        TestPbwtSandpit.cpp)

add_executable(unit_tests ${test_src})
target_link_libraries(unit_tests PUBLIC data_module_lib project_warnings project_settings Eigen3::Eigen fmt::fmt Catch2::Catch2WithMain)

# automatically discover tests
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
