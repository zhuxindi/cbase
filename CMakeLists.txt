cmake_minimum_required(VERSION 3.10)
project(cbase C)

set(CMAKE_C_STANDARD 99)
option(WITH_TEST "compile test examples" OFF)

add_definitions(-fPIC -pipe -O2 -W -Wall -Wpointer-arith -Wno-unused-parameter -D_GNU_SOURCE)

aux_source_directory(src SRC_LIST)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE include)

if(WITH_TEST)
  add_executable(utils_test test/utils_test.c)
  target_include_directories(utils_test PRIVATE include)
  target_link_libraries(utils_test PRIVATE ${PROJECT_NAME})

  add_executable(log_test test/log_test.c)
  target_include_directories(log_test PRIVATE include)
  target_link_libraries(log_test PRIVATE ${PROJECT_NAME})

  add_executable(event_test test/event_test.c)
  target_include_directories(event_test PRIVATE include)
  target_link_libraries(event_test PRIVATE ${PROJECT_NAME})

  add_executable(buffer_test test/buffer_test.c)
  target_include_directories(buffer_test PRIVATE include)
  target_link_libraries(buffer_test PRIVATE ${PROJECT_NAME})

  add_executable(rbtree_test test/rbtree_test.c)
  target_include_directories(rbtree_test PRIVATE include)
  target_link_libraries(rbtree_test PRIVATE ${PROJECT_NAME})

  add_executable(stream_test test/stream_test.c)
  target_include_directories(stream_test PRIVATE include)
  target_link_libraries(stream_test PRIVATE ${PROJECT_NAME})

  add_executable(pool_test test/pool_test.c)
  target_include_directories(pool_test PRIVATE include)
  target_link_libraries(pool_test PRIVATE ${PROJECT_NAME})

  add_executable(xstrings_test test/xstrings_test.c)
  target_include_directories(xstrings_test PRIVATE include)
  target_link_libraries(xstrings_test PRIVATE ${PROJECT_NAME})

  add_executable(ring_test test/ring_test.c)
  target_include_directories(ring_test PRIVATE include)
  target_link_libraries(ring_test PRIVATE ${PROJECT_NAME})
endif()