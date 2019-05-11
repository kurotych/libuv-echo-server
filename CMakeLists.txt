cmake_minimum_required(VERSION 3.12)
project(proto_server C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(proto_server main.c)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(${PROJECT_NAME}  PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glib-2.0 ev uv)