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
# Examples
# ------------------------
add_executable(cvl_demo examples/demo.c)
target_link_libraries(cvl_demo cvl)
