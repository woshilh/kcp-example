include_directories(${CMAKE_SOURCE_DIR}/entity)
include_directories(${CMAKE_SOURCE_DIR}/kcp)
include_directories(${CMAKE_SOURCE_DIR}/taskqueue)
include_directories(${CMAKE_SOURCE_DIR}/base)
set(EXECUTABLE_NAME "t_test")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/test/t_encoder.cc)
target_link_libraries(${EXECUTABLE_NAME} entity taskqueue base pthread)
