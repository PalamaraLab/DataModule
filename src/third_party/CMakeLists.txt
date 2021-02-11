set(pandas_plink_src pandas_plink/bed_reader.c)
set(pandas_plink_hrd pandas_plink/bed_reader.h)

add_library(pandas_plink_lib STATIC ${pandas_plink_src} ${pandas_plink_hrd})
target_include_directories(pandas_plink_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
