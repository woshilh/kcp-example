include_directories(${CMAKE_SOURCE_DIR}/kcp)
set(kcp_FILES
ikcp.c
)
add_library(kcp STATIC ${kcp_FILES})
