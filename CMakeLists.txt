cmake_minimum_required(VERSION 3.26.4)
project(pel)

include_directories(lib)

add_library(
    ${PROJECT_NAME}
    src/pel.c
)

add_subdirectory(lib/png-easy)
target_link_libraries(${PROJECT_NAME} png-easy)