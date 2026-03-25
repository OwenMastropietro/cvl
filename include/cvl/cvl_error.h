#pragma once

#include <stdarg.h>
#include <stdio.h>

// todo: this felt like a good idea until I started implementing it...

// status codes
typedef enum cvl_status_t {
    CVL_OK,
    CVL_ERR_INVALID_ARG,
    CVL_ERR_INVALID_DIM,
    CVL_ERR_TYPE_MISMATCH,
    CVL_ERR_OUT_OF_MEMORY,
    CVL_ERR_FILE_IO,
    CVL_ERR_EMPTY,
    CVL_ERR_OUT_OF_RANGE,
    CVL_ERR_INTERNAL,
} cvl_status_t;

// Maps status codes to status messages.
const char *cvl_status_string(cvl_status_t s);

// Print status message and return status code.
cvl_status_t cvl_perrorf(cvl_status_t code, const char *fmt, ...);
