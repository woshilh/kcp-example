include_directories(${CMAKE_SOURCE_DIR}/kcp)
include_directories(${CMAKE_SOURCE_DIR}/taskqueue)
include_directories(${CMAKE_SOURCE_DIR}/base)
set(entity_FILES
proto.c
fakefilereader.cc
kcpstream.cc
)
add_library(entity STATIC ${entity_FILES})
