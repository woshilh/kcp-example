include_directories(${CMAKE_SOURCE_DIR}/base)
set(base_FILES
timeutils.cc
lock.cc
)
add_library(base STATIC ${base_FILES})
