project(test)
cmake_minimum_required(VERSION 3.10)


set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})


FILE(COPY cacert.pem DESTINATION ${CMAKE_SOURCE_DIR}/build/test/)

FILE(COPY privkey.pem DESTINATION ${CMAKE_SOURCE_DIR}/build/test/)

add_executable(test_xxx for_test.c)

find_package(OpenSSL REQUIRED)
target_link_libraries(test_xxx OpenSSL::SSL)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GLIB REQUIRED glib-2.0)
include_directories (${GLIB_INCLUDE_DIRS})
link_directories (${GLIB_INCLUDE_DIRS})
add_definitions (${GLIB_CFLAGS_OTHER})
target_link_libraries (test_xxx ${GLIB_LIBRARIES})
