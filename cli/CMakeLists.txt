add_executable(cvl
    main.c
    cmd_blur.c
)
target_link_libraries(cvl PRIVATE cvl_lib cvl_warnings)
