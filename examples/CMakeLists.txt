set(CVL_EXAMPLES
    cs136
    color
    threshold
    blur
    ccl
    sobel
    resize
    rotate
)

foreach(example ${CVL_EXAMPLES})
    add_executable(cvl_${example} ${example}.c)
    target_link_libraries(cvl_${example} PRIVATE cvl_lib cvl_warnings)
endforeach()
