cmake_minimum_required(VERSION 3.16)

project(cvl)

set(CMAKE_C_STANDARD 11)

add_library(cvl
    src/core.c
    src/imgproc.c
    src/io.c
)

target_link_libraries(cvl PUBLIC m)

target_include_directories(cvl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # todo: install interface
)

target_compile_options(cvl PRIVATE -Wall -Wextra)

# =================
# Example Programs
# =================

add_executable(cvl_cs136 examples/cs136.c)
target_link_libraries(cvl_cs136 cvl)

add_executable(cvl_color examples/color.c)
target_link_libraries(cvl_color cvl)

add_executable(cvl_threshold examples/threshold.c)
target_link_libraries(cvl_threshold cvl)

add_executable(cvl_blur examples/blur.c)
target_link_libraries(cvl_blur cvl)

add_executable(cvl_ccl examples/ccl.c)
target_link_libraries(cvl_ccl cvl)

add_executable(cvl_sobel examples/sobel.c)
target_link_libraries(cvl_sobel cvl)

# Optionally Build Tests.
option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
