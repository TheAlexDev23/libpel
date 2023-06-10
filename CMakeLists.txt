cmake_minimum_required(VERSION 3.26.0)
project(pel)

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

include_directories(include/pel)
include_directories(src)

add_library(
    ${PROJECT_NAME}
    src/pel.c
    src/handle.h
    src/handle.c
    src/error.c
    src/image.h
    src/helper/png-easy.c
    src/helper/png-easy.h
    src/helper/ft-easy.c
    src/helper/ft-easy.h
    src/pixel.c
    src/pixel.h
    src/state.h
    src/shapes/circle.c
    src/shapes/line.c
    src/shapes/text.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE REQUIRED freetype2)

include_directories(${FREETYPE_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FREETYPE_CFLAGS})

target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

target_link_libraries(${PROJECT_NAME} png)


include(tests/tests.cmake)
add_subdirectory(tests)

foreach (element IN LISTS tests)
    add_test(NAME ${element} COMMAND tests/${element})
endforeach()