cmake_minimum_required(VERSION 3.16)

project(cvl)

set(CMAKE_C_STANDARD 11)

add_library(cvl_lib
    src/core.c
    src/imgproc.c
    src/io.c
    src/stb.c
)

target_link_libraries(cvl_lib PUBLIC m)

target_include_directories(cvl_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty>
    # todo: install interface
)

target_compile_options(cvl_lib PRIVATE -Wall -Wextra)

# =================
# CLI
# =================
add_executable(cvl
    src/cli/main.c
    src/cli/cmd_blur.c
)
target_link_libraries(cvl PRIVATE cvl_lib)

# =================
# Example Programs
# =================

add_executable(cvl_cs136 examples/cs136.c)
target_link_libraries(cvl_cs136 cvl_lib)

add_executable(cvl_color examples/color.c)
target_link_libraries(cvl_color cvl_lib)

add_executable(cvl_threshold examples/threshold.c)
target_link_libraries(cvl_threshold cvl_lib)

add_executable(cvl_blur examples/blur.c)
target_link_libraries(cvl_blur cvl_lib)

add_executable(cvl_ccl examples/ccl.c)
target_link_libraries(cvl_ccl cvl_lib)

add_executable(cvl_sobel examples/sobel.c)
target_link_libraries(cvl_sobel cvl_lib)

add_executable(cvl_resize examples/resize.c)
target_link_libraries(cvl_resize cvl_lib)

# Optionally Build Tests.
option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
