cmake_minimum_required(VERSION 3.26.0)
project(pel)

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(src)

add_library(
    ${PROJECT_NAME}
    src/pel.c
    src/handle.h
    src/handle.c
    src/image.c
    src/image.h
    src/png-easy.c
    src/png-easy.h
    src/pixel.c
    src/pixel.h
    src/shapes/circle.c
)

target_link_libraries(${PROJECT_NAME} png)

add_subdirectory(tests)
add_test(NAME Circle COMMAND tests/circle)