cmake_minimum_required(VERSION 3.16)

project(cvl)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(cvl
    src/cvl_core.c
    src/cvl_imgproc.c
    src/cvl_hough.c
    src/cvl_io.c
    src/cvl_error.c
    src/cvl_draw.c
)

if(UNIX AND NOT APPLE)
    target_link_libraries(cvl PUBLIC m)
endif()

target_include_directories(cvl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # todo: install interface
)

if (MSVC)
    target_compile_options(cvl PRIVATE /W4)
else()
    target_compile_options(cvl PRIVATE -Wall -Wextra -pedantic)
endif()

# Example Programs
file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS examples/*.c)
foreach(example ${EXAMPLE_SOURCES})
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example})
    target_link_libraries(${name} cvl)

    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/example
    )
endforeach()

# Optionally Build Tests.
option(BUILD_TESTS "Build tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
