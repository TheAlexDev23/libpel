cmake_minimum_required(VERSION 3.26.4)
project(pel)

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

add_library(
    ${PROJECT_NAME}
    src/pel.c
    src/png-easy.c
    src/png-easy.h
)

target_link_libraries(${PROJECT_NAME} png)

add_subdirectory(tests)
add_test(NAME Circle COMMAND tests/circle)