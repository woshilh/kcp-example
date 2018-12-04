include_directories(${CMAKE_SOURCE_DIR}/kcp)
set(entity_FILES
proto.c
fakefilereader.cc
kcpstream.cc
)
add_library(entity STATIC ${entity_FILES})
