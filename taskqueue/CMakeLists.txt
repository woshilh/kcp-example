include_directories(${CMAKE_SOURCE_DIR}/base)
include_directories(${CMAKE_SOURCE_DIR}/taskqueue/)
set(task_queue_FILES
platform_thread.cc 
platform_thread_types.cc
criticalsection.cc
taskqueue.cc
)
add_library(taskqueue STATIC ${task_queue_FILES})
