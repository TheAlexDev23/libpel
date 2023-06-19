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
    src/helper/jpeg-easy.c
    src/helper/jpeg-easy.h
    src/helper/webp-easy.c
    src/helper/webp-easy.h
    src/helper/ft-easy.c
    src/helper/ft-easy.h
    src/pixel.c
    src/pixel.h
    src/state.h
    src/shapes/circle.c
    src/shapes/line.c
    src/shapes/line.h
    src/shapes/rectangle.c
    src/shapes/text.c
    src/shapes/triangle.c
    src/filters.c
)

find_package(PkgConfig REQUIRED)

set(pkgs freetype2 fontconfig libpng libjpeg libwebp)
set(pkg_names FREETYPE FONTCONFIG PNG JPEG WEBP)
set(idx 0)

foreach (PACKAGE IN LISTS pkgs)
    list(GET pkg_names ${idx} PKG)
    pkg_check_modules(${PKG} REQUIRED ${PACKAGE})
    foreach(LIB IN LISTS ${PKG}_LIBRARIES)
        target_link_libraries(${PROJECT_NAME} ${LIB})
    endforeach()

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

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

# example usage
enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

include(tests/tests.cmake)
add_subdirectory(tests)

foreach (element IN LISTS tests)
    add_test(NAME ${element} COMMAND tests/${element})
endforeach()
