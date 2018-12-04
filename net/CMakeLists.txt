include_directories(${CMAKE_SOURCE_DIR}/net)
set(net_FILES
posix.c
)
add_library(net STATIC ${net_FILES})
