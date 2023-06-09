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
    src/image.h
    src/helper/png-easy.c
    src/helper/png-easy.h
    src/pixel.c
    src/pixel.h
    src/state.h
    src/shapes/circle.c
    src/shapes/line.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE REQUIRED freetype2)

include_directories(${TRUETYPE_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${TRUETYPE_CFLAGS})

target_link_libraries(${PROJECT_NAME} ${TRUETYPE_LIBRARIES})

target_link_libraries(${PROJECT_NAME} png)

add_subdirectory(tests)
add_test(NAME Circle COMMAND tests/circle)
add_test(NAME Line COMMAND tests/line)