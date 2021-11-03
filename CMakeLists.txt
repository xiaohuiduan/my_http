cmake_minimum_required(VERSION 3.10)
project(my_http C)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "-std=c99 -g -pthread")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "-std=c99 -O2 -pthread")
endif ()

set(src
        ${CMAKE_CURRENT_SOURCE_DIR}/server.c
        ${CMAKE_CURRENT_SOURCE_DIR}/request.c
        ${CMAKE_CURRENT_SOURCE_DIR}/response.c
        ${CMAKE_CURRENT_SOURCE_DIR}/thread_process_http.c
        ${CMAKE_CURRENT_SOURCE_DIR}/response_util.c)

#include_directories(${CMAKE})
#add_executable(test test.c)
add_executable(my_http  ${src} response_util.c response_util.h)

### dependencies
#INCLUDE(FindPkgConfig)
### 使用pkg-config寻找glib-2.0和gobject-2.0库的include目录以及库

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(my_http ${GLIB_LIBRARIES})