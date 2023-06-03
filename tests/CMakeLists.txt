cmake_minimum_required(VERSION 3.26.0)
project(tests)

include_directories(../include)

add_executable(circle circle.c)

target_link_libraries(circle pel)