# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/7917641ff965959afae189afb5f052524395525c.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(test_cvl
  test_image.cpp
)
target_link_libraries(test_cvl PRIVATE
    cvl
    gtest_main
)

target_include_directories(test_cvl PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(test_cvl PRIVATE
    TEST_SOURCE_DIR="${CMAKE_SOURCE_DIR}/tests"
)

# Register tests
include(GoogleTest)
gtest_discover_tests(test_cvl)
