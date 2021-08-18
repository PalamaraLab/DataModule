include(CMakeFindDependencyMacro)

find_dependency(Eigen3)
find_dependency(fmt)
find_dependency(range-v3)
find_dependency(ZLIB)

include(${CMAKE_CURRENT_LIST_DIR}/asmc-data-module-runtime.cmake)
