cmake_minimum_required(VERSION 3.12)
project(echo_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(${PROJECT_NAME} main.c udp_echo.c tcp_echo.c)

target_link_libraries(${PROJECT_NAME} uv)