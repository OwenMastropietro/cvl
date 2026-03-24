cmake_minimum_required(VERSION 3.16)

project(cvl)

set(CMAKE_C_STANDARD 11)

add_library(cvl
    src/cvl_core.c
    src/cvl_imgproc.c
    src/cvl_io.c
)

target_include_directories(cvl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # todo: install interface
)

target_compile_options(cvl PRIVATE -Wall -Wextra)

# ------------------------
# Example Programs
# ------------------------
add_executable(cvl_cs136 examples/cs136.c)
target_link_libraries(cvl_cs136 cvl)

add_executable(cvl_canny examples/canny.c)
target_link_libraries(cvl_canny cvl)

add_executable(cvl_sobel examples/sobel.c)
target_link_libraries(cvl_sobel cvl)

add_executable(cvl_ccl examples/ccl.c)
target_link_libraries(cvl_ccl cvl)

# Optionally Build Tests.
option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
