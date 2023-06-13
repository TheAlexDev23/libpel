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
    src/image.c
    src/helper/png-easy.c
    src/helper/png-easy.h
    src/helper/ft-easy.c
    src/helper/ft-easy.h
    src/pixel.c
    src/pixel.h
    src/state.h
    src/shapes/circle.c
    src/shapes/line.c
    src/shapes/line.h
    src/shapes/text.c
    src/shapes/triangle.c
)

find_package(PkgConfig REQUIRED)

set(pkgs freetype2 fontconfig libpng)
set(pkg_names FREETYPE FONTCONFIG PNG)
set(idx 0)

foreach (PACKAGE IN LISTS pkgs)
    list(GET pkg_names ${idx} PKG)
    pkg_check_modules(${PKG} REQUIRED ${PACKAGE})
    foreach (DIR IN LISTS ${PKG}_INCLUDE_DIRS)
        include_directories(${DIR})
    endforeach()

    foreach (FLAG IN LISTS ${PKG}_CFLAGS)
        target_compile_options(${PROJECT_NAME} PUBLIC ${FLAG})
    endforeach()

    string(TOLOWER "${PKG}" PKG)
    target_link_libraries(${PROJECT_NAME} ${PKG})

    math(EXPR idx "${idx} + 1")
endforeach()

target_link_libraries(${PROJECT_NAME} m)

include(tests/tests.cmake)
add_subdirectory(tests)

foreach (element IN LISTS tests)
    add_test(NAME ${element} COMMAND tests/${element})
endforeach()
