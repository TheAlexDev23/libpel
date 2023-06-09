cmake_minimum_required(VERSION 3.26.0)
project(tests)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PEL REQUIRED pel)

set(tests circle line)
foreach (element IN LISTS tests)
    add_executable(${element} ${element}.c)
    target_compile_options(${element} PUBLIC ${PEL_CFLAGS})
    target_link_libraries(${element} ${PEL_LIBRARIES})
endforeach()
