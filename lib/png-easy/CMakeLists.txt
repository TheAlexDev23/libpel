cmake_minimum_required(VERSION 3.26.4)
project(png-easy)

enable_testing()

add_library(
    ${PROJECT_NAME}
    png-easy.h
    png-easy.c
    png-io.c
    png-io.h
    )

target_link_libraries(${PROJECT_NAME} png)

# Testing

add_subdirectory(tests)
add_test(NAME Create_Empty_png COMMAND tests/creating)