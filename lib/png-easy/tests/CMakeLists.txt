cmake_minimum_required(VERSION 3.26.4)
project(tests)

include_directories(..)

add_executable(creating creating.c)
target_link_libraries(creating png-easy)