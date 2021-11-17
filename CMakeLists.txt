cmake_minimum_required(VERSION 3.10)
project(my_http C)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "-std=c99 -g -pthread")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "-std=c99 -O2 -pthread")
endif ()

set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
include_directories(include)
aux_source_directory(${CMAKE_SOURCE_DIR}/src proj_src)
add_executable(${PROJECT_NAME} ${proj_src})

 add_subdirectory(test)

FILE(COPY res DESTINATION "${CMAKE_BINARY_DIR}")

find_package(OpenSSL REQUIRED)
target_link_libraries(my_http OpenSSL::SSL)


find_package (PkgConfig REQUIRED)
pkg_check_modules (GLIB REQUIRED glib-2.0)
include_directories (${GLIB_INCLUDE_DIRS})
link_directories (${GLIB_INCLUDE_DIRS})
add_definitions (${GLIB_CFLAGS_OTHER})
target_link_libraries (my_http ${GLIB_LIBRARIES})

set(INC_DIR /usr/include)
set(LINK_DIR /usr/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(event)
target_link_libraries(my_http event)