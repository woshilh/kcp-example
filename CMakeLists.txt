PROJECT(project)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++11)

add_subdirectory(kcp)
add_subdirectory(net)
add_subdirectory(entity)
add_subdirectory(test)

include_directories(${CMAKE_SOURCE_DIR}/kcp)
include_directories(${CMAKE_SOURCE_DIR}/net)
include_directories(${CMAKE_SOURCE_DIR}/entity)

set(EXECUTABLE_NAME "t_client")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/client.cc)
target_link_libraries(${EXECUTABLE_NAME} entity net kcp pthread)

set(EXECUTABLE_NAME "t_server")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/server.cc)
target_link_libraries(${EXECUTABLE_NAME} entity net kcp pthread)
