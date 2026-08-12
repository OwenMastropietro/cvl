cmake_minimum_required(VERSION 3.16)

project(cvl VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(cvl_lib
    src/core.c
    src/imgproc.c
    src/io.c
    src/stb.c
)

target_link_libraries(cvl_lib PUBLIC m)

target_include_directories(cvl_lib
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)

add_library(cvl_warnings INTERFACE)
target_compile_options(cvl_warnings INTERFACE -Wall -Wextra)

# Build examples by default.
option(BUILD_EXAMPLES "Build example programs" OFF)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build CLI by default.
option(BUILD_CLI "Build the cvl command-line tool" ON)
if (BUILD_CLI)
    add_subdirectory(cli)
endif()

# Build Tests if enabeld.
option(BUILD_TESTS "Build tests" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation.
set(CVL_INSTALL_TARGETS cvl_lib)
if (BUILD_CLI)
    list(APPEND CVL_INSTALL_TARGETS cvl)
endif()

install(TARGETS ${CVL_INSTALL_TARGETS}
    EXPORT cvlTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(EXPORT cvlTargets NAMESPACE cvl:: FILE cvlConfig.cmake DESTINATION lib/cmake/cvl)
install(DIRECTORY include/cvl DESTINATION include)