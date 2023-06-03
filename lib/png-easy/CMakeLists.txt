cmake_minimum_required(VERSION 3.26.4)

project(png-easy)

add_library(
    ${PROJECT_NAME}
    png-easy.h
    png-easy.c
    png-io.c
    png-io.h
    )

target_link_libraries(${PROJECT_NAME} png)