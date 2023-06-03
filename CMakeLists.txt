cmake_minimum_required(VERSION 3.26.4)
project(pel)

add_library(
    ${PROJECT_NAME}
    src/pel.c
    src/png-easy.c
    src/png-easy.h
)

target_link_libraries(${PROJECT_NAME} png)